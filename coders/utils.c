

# include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(long ms, t_sim *sim)
{
	long	start;

	start = get_time_ms();
	while (1)
	{
		pthread_mutex_lock(&sim->stop_mutex);
		if (sim->stop)
		{
			pthread_mutex_unlock(&sim->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&sim->stop_mutex);
		if ((get_time_ms() - start) >= ms)
			break ;
		usleep(500);
	}
}
