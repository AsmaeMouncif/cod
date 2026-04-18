# include "codexion.h"

static void	fill_args(t_args *ac, char **av)
{
	ac->number_of_coders = atoi(av[1]);
	ac->time_to_burnout = atoi(av[2]);
	ac->time_to_compile = atoi(av[3]);
	ac->time_to_debug = atoi(av[4]);
	ac->time_to_refactor = atoi(av[5]);
	ac->number_of_compiles_required = atoi(av[6]);
	ac->dongle_cooldown = atoi(av[7]);
	ac->scheduler = av[8];
}

static int	init_dongles(t_sim *sim)
{
	int	i;
	int	n;

	n = sim->args.number_of_coders;
	sim->dongles = malloc(sizeof(t_dongle) * n);
	if (!sim->dongles)
		return (1);
	i = 0;
	while (i < n)
	{
		sim->dongles[i].in_use = 0;
		sim->dongles[i].last_released = 0;
		sim->dongles[i].waitlist = NULL;
		pthread_mutex_init(&sim->dongles[i].mutex, NULL);
		pthread_cond_init(&sim->dongles[i].cond, NULL);
		i++;
	}
	return (0);
}

static int	init_coders(t_sim *sim)
{
	int	i;
	int	n;

	n = sim->args.number_of_coders;
	sim->coders = malloc(sizeof(t_coder) * n);
	if (!sim->coders)
		return (1);
	i = 0;
	while (i < n)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_count = 0;
		sim->coders[i].last_compile_time = sim->start_time;
		sim->coders[i].left = &sim->dongles[i];
		sim->coders[i].right = &sim->dongles[(i + 1) % n];
		sim->coders[i].sim = sim;
		i++;
	}
	return (0);
}

t_sim	*init_sim(char **av)
{
	t_sim	*sim;

	sim = malloc(sizeof(t_sim));
	if (!sim)
		return (NULL);
	fill_args(&sim->args, av);
	sim->stop = 0;
	sim->start_time = get_time_ms();
	pthread_mutex_init(&sim->log_mutex, NULL);
	pthread_mutex_init(&sim->stop_mutex, NULL);
	if (init_dongles(sim))
	{
		free(sim);
		return (NULL);
	}
	if (init_coders(sim))
	{
		free(sim->dongles);
		free(sim);
		return (NULL);
	}
	return (sim);
}

void	free_sim(t_sim *sim)
{
	int			i;
	int			n;
	t_waitlist	*wl;

	if (!sim)
		return ;
	n = sim->args.number_of_coders;
	i = 0;
	while (i < n)
	{
		if (sim->dongles[i].waitlist)
		{
			wl = (t_waitlist *)sim->dongles[i].waitlist;
			free(wl->coders);
			free(wl);
		}
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		pthread_cond_destroy(&sim->dongles[i].cond);
		i++;
	}
	free(sim->dongles);
	free(sim->coders);
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_mutex_destroy(&sim->stop_mutex);
	free(sim);
}
