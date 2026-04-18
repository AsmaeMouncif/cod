# include "codexion.h"

t_waitlist	*waitlist_create(void)
{
	t_waitlist	*wl;

	wl = malloc(sizeof(t_waitlist));
	if (!wl)
		return (NULL);
	wl->coders = malloc(sizeof(t_coder *) * 64);
	if (!wl->coders)
	{
		free(wl);
		return (NULL);
	}
	wl->size = 0;
	wl->capacity = 64;
	return (wl);
}

void	waitlist_add(t_waitlist *wl, t_coder *coder)
{
	if (wl->size < wl->capacity)
	{
		wl->coders[wl->size] = coder;
		wl->size++;
	}
}

void	waitlist_remove(t_waitlist *wl, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < wl->size)
	{
		if (wl->coders[i] == coder)
		{
			wl->coders[i] = wl->coders[wl->size - 1];
			wl->size--;
			return ;
		}
		i++;
	}
}

static t_coder	*get_next_fifo(t_waitlist *wl)
{
	if (wl->size == 0)
		return (NULL);
	return (wl->coders[0]);
}

static t_coder	*get_next_edf(t_waitlist *wl, t_sim *sim)
{
	t_coder	*best;
	long	best_deadline;
	long	deadline;
	int		i;

	if (wl->size == 0)
		return (NULL);
	best = wl->coders[0];
	best_deadline = best->last_compile_time
		+ sim->args.time_to_burnout;
	i = 1;
	while (i < wl->size)
	{
		deadline = wl->coders[i]->last_compile_time
			+ sim->args.time_to_burnout;
		if (deadline < best_deadline)
		{
			best = wl->coders[i];
			best_deadline = deadline;
		}
		i++;
	}
	return (best);
}

t_coder	*get_next_coder(t_waitlist *wl, t_sim *sim)
{
	if (strcmp(sim->args.scheduler, "fifo") == 0)
		return (get_next_fifo(wl));
	return (get_next_edf(wl, sim));
}
