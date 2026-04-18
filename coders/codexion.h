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

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_sim    t_sim;

typedef struct s_args
{
    int number_of_coders;
	int time_to_burnout;
	int time_to_compile;
	int time_to_debug;
	int time_to_refactor;
	int number_of_compiles_required;
	int dongle_cooldown;
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

typedef struct s_waitlist
{
	t_coder		**coders;
	int			size;
	int			capacity;
}	t_waitlist;


int is_valid_number(char *str);
int	is_valid_scheduler(char *str);
t_sim	*init_sim(char **av);
void	free_sim(t_sim *sim);
long	get_time_ms(void);
void	ft_usleep(long ms, t_sim *sim);
void	log_state(t_sim *sim, int id, char *state);
void	*coder_routine(void *arg);
void	take_dongle(t_dongle *dongle, t_coder *coder);
void	release_dongle(t_dongle *dongle, t_sim *sim);
void	*monitor_routine(void *arg);
t_waitlist	*waitlist_create(void);
void	waitlist_add(t_waitlist *wl, t_coder *coder);
void	waitlist_remove(t_waitlist *wl, t_coder *coder);
t_coder	*get_next_coder(t_waitlist *wl, t_sim *sim);
int	try_take_dongle(t_dongle *dongle, t_coder *coder);

#endif
