# include "codexion.h"

void	log_state(t_sim *sim, int id, char *state)
{
	long	timestamp;

	pthread_mutex_lock(&sim->log_mutex);
	pthread_mutex_lock(&sim->stop_mutex);
	if (sim->stop)
	{
		pthread_mutex_unlock(&sim->stop_mutex);
		pthread_mutex_unlock(&sim->log_mutex);
		return ;
	}
	pthread_mutex_unlock(&sim->stop_mutex);
	timestamp = get_time_ms() - sim->start_time;
	printf("%ld %d %s\n", timestamp, id, state);
	pthread_mutex_unlock(&sim->log_mutex);
}
