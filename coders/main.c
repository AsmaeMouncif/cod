/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 16:36:13 by asmounci          #+#    #+#             */
/*   Updated: 2026/04/16 18:16:27 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

int	main(int ac, char **av)
{
	int	i;
	t_sim	*sim;

	if (ac != 9)
	{
		write(2, "Error: invalid number of arguments\n", 35);
		return (1);
	}

	i = 1;
    while (i <= 7)
    {
        if (!is_valid_number(av[i]))
		{
			write(2, "Error: invalid numeric argument\n", 32);
			return (1);
		}
        i++;
    }

	if (!is_valid_scheduler(av[8]))
	{
		write(2, "Error: invalid scheduler\n", 25);
		return (1);
	}

	sim = init_sim(av);
	if (!sim)
	{
		write(2, "Error: init failed\n", 19);
		return (1);
	}
	pthread_create(&sim->monitor, NULL, monitor_routine, sim);
	i = 0;
	while (i < sim->args.number_of_coders)
	{
		pthread_create(&sim->coders[i].thread, NULL,
			coder_routine, &sim->coders[i]);
		i++;
	}
	i = 0;
	while (i < sim->args.number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
	free_sim(sim);
	return (0);
}
