# include "codexion.h"

static int	cooldown_passed(t_dongle *dongle, t_sim *sim)
{
	return ((get_time_ms() - dongle->last_released)
		>= sim->args.dongle_cooldown);
}

void	take_dongle(t_dongle *dongle, t_coder *coder)
{
	t_waitlist	*wl;
	t_sim		*sim;
	int			stop;

	sim = coder->sim;
	pthread_mutex_lock(&dongle->mutex);
	if (!dongle->waitlist)
		dongle->waitlist = waitlist_create();
	wl = (t_waitlist *)dongle->waitlist;
	waitlist_add(wl, coder);
	while (1)
	{
		pthread_mutex_unlock(&dongle->mutex);
		pthread_mutex_lock(&sim->stop_mutex);
		stop = sim->stop;
		pthread_mutex_unlock(&sim->stop_mutex);
		pthread_mutex_lock(&dongle->mutex);
		if (stop)
		{
			waitlist_remove(wl, coder);
			pthread_mutex_unlock(&dongle->mutex);
			return ;
		}
		if (!dongle->in_use && cooldown_passed(dongle, sim)
			&& get_next_coder(wl, sim) == coder)
		{
			dongle->in_use = 1;
			waitlist_remove(wl, coder);
			pthread_mutex_unlock(&dongle->mutex);
			log_state(sim, coder->id, "has taken a dongle");
			return ;
		}
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
}

void	release_dongle(t_dongle *dongle, t_sim *sim)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->in_use = 0;
	dongle->last_released = get_time_ms();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
	(void)sim;
}

int	try_take_dongle(t_dongle *dongle, t_coder *coder)
{
	t_waitlist	*wl;
	t_sim		*sim;
	int			stop;
	int			result;

	sim = coder->sim;
	result = 0;
	pthread_mutex_lock(&dongle->mutex);
	if (!dongle->waitlist)
		dongle->waitlist = waitlist_create();
	wl = (t_waitlist *)dongle->waitlist;
	pthread_mutex_lock(&sim->stop_mutex);
	stop = sim->stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	if (stop)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	waitlist_add(wl, coder);
	if (!dongle->in_use
		&& ((get_time_ms() - dongle->last_released)
			>= sim->args.dongle_cooldown)
		&& get_next_coder(wl, sim) == coder)
	{
		dongle->in_use = 1;
		waitlist_remove(wl, coder);
		pthread_mutex_unlock(&dongle->mutex);
		log_state(sim, coder->id, "has taken a dongle");
		return (1);
	}
	waitlist_remove(wl, coder);
	pthread_mutex_unlock(&dongle->mutex);
	return (result);
}
