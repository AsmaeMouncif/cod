/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 16:36:32 by asmounci          #+#    #+#             */
/*   Updated: 2026/04/16 19:26:28 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>

typedef struct s_sim t_sim;

typedef struct s_args
{
    int	number_of_coders;
	int	time_to_burnout;
	int	time_to_compile;
	int	time_to_debug;
	int	time_to_refactor;
	int	number_of_compiles_required;
	int	dongle_cooldown;
	char *scheduler;
}	t_args;

typedef struct s_dongle
{
	int in_use;
    long last_released;
    void *waitlist;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}	t_dongle;

typedef struct s_coder
{
	int id;
    int compile_count;
    long last_compile_time;
    t_dongle *left;
    t_dongle *right;
    pthread_t thread;
    t_sim *sim;
}	t_coder;

typedef struct s_sim
{
	t_coder *coders;
    t_dongle *dongles;
    t_args args;
    int stop;
    pthread_mutex_t log_mutex;
    pthread_mutex_t stop_mutex;
    long start_time;
    pthread_t monitor;
}	t_sim;


int is_valid_number(char *str);
int	is_valid_scheduler(char *str);

#endif
