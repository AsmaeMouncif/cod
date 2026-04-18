# include "codexion.h"

static void	broadcast_all(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		i++;
	}
}

static void	set_stop(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	broadcast_all(sim);
}

static int	check_burnout(t_sim *sim)
{
	int		i;
	long	elapsed;
	long	timestamp;

	i = 0;
	while (i < sim->args.number_of_coders)
	{
		pthread_mutex_lock(&sim->stop_mutex);
		elapsed = get_time_ms() - sim->coders[i].last_compile_time;
		pthread_mutex_unlock(&sim->stop_mutex);
		if (elapsed >= sim->args.time_to_burnout)
		{
			pthread_mutex_lock(&sim->log_mutex);
			timestamp = get_time_ms() - sim->start_time;
			printf("%ld %d burned out\n", timestamp, sim->coders[i].id);
			pthread_mutex_unlock(&sim->log_mutex);
			set_stop(sim);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	check_done(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.number_of_coders)
	{
		if (sim->coders[i].compile_count
			< sim->args.number_of_compiles_required)
			return (0);
		i++;
	}
	set_stop(sim);
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		stop;

	sim = (t_sim *)arg;
	while (1)
	{
		pthread_mutex_lock(&sim->stop_mutex);
		stop = sim->stop;
		pthread_mutex_unlock(&sim->stop_mutex);
		if (stop)
			break ;
		if (check_burnout(sim))
			break ;
		if (check_done(sim))
			break ;
		usleep(1000);
	}
	return (NULL);
}
