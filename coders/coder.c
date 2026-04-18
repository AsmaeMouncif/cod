# include "codexion.h"

static int	is_stopped(t_coder *coder)
{
	int	stop;

	pthread_mutex_lock(&coder->sim->stop_mutex);
	stop = coder->sim->stop;
	pthread_mutex_unlock(&coder->sim->stop_mutex);
	return (stop);
}

static void	do_refactor(t_coder *coder)
{
	if (is_stopped(coder))
		return ;
	log_state(coder->sim, coder->id, "is refactoring");
	ft_usleep(coder->sim->args.time_to_refactor, coder->sim);
}

static void	do_debug(t_coder *coder)
{
	if (is_stopped(coder))
		return ;
	log_state(coder->sim, coder->id, "is debugging");
	ft_usleep(coder->sim->args.time_to_debug, coder->sim);
}

static void	pickup_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	int			n;

	n = coder->sim->args.number_of_coders;
	if (coder->id == n)
	{
		first = coder->right;
		second = coder->left;
	}
	else
	{
		first = coder->left;
		second = coder->right;
	}
	while (!is_stopped(coder))
	{
		take_dongle(first, coder);
		if (is_stopped(coder))
			return ;
		if (try_take_dongle(second, coder))
			return ;
		release_dongle(first, coder->sim);
		ft_usleep(2, coder->sim);
	}
}

static void	do_compile(t_coder *coder)
{
	if (is_stopped(coder))
		return ;
	pickup_dongles(coder);
	if (is_stopped(coder))
		return ;
	pthread_mutex_lock(&coder->sim->stop_mutex);
	coder->last_compile_time = get_time_ms();
	pthread_mutex_unlock(&coder->sim->stop_mutex);
	log_state(coder->sim, coder->id, "is compiling");
	ft_usleep(coder->sim->args.time_to_compile, coder->sim);
	coder->compile_count++;
	release_dongle(coder->left, coder->sim);
	release_dongle(coder->right, coder->sim);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->sim->args.number_of_coders == 1)
	{
		log_state(coder->sim, coder->id, "has taken a dongle");
		ft_usleep(coder->sim->args.time_to_burnout, coder->sim);
		return (NULL);
	}
	if (coder->id % 2 == 0)
		ft_usleep(1, coder->sim);
	while (!is_stopped(coder))
	{
		do_compile(coder);
		if (is_stopped(coder))
			break ;
		do_debug(coder);
		if (is_stopped(coder))
			break ;
		do_refactor(coder);
	}
	return (NULL);
}
