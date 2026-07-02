/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospina- <lospina-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 13:28:37 by lospina-          #+#    #+#             */
/*   Updated: 2026/07/02 17:53:05 by lospina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEX_H
# define CODEX_H

//**LIBRARIES */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>
# include <string.h>

//** STRUCTURES DECLARATION */
typedef struct s_dongle		t_dongle;
typedef struct s_coder		t_coder;
typedef struct s_desk		t_desk;
typedef struct s_queue		t_queue;
typedef struct s_queue_item	t_queue_item;

//**DONGLE QUEUE */
struct	s_queue_item
{
	int			coder_id;
	long long	priority;
};

struct	s_queue
{
	int				size;
	t_queue_item	*array;
};

//**  DONGLES*/
struct s_dongle
{
	int				holding_coder_id;
	int				dongle_id;
	int				cooldown_wait;
	long			free_at;
	t_queue			dongle_queue;
	pthread_cond_t	cond;
	pthread_mutex_t	mutex;
};

//** CODERS */
struct s_coder
{
	int			id;
	long		compiler_counter;
	long		last_comp_time;
	bool		comp_done;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	t_desk		*desk;
	pthread_t	thread_id;
};

//**CO-WORKING DESK */
struct s_desk
{
	long			number_of_coders;
	long			number_of_compiles_required;
	long			start_time;
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	long long		dongle_cooldown;
	unsigned long	start_simulation;
	char			*scheduler;
	bool			end_simulation;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	sim_mutex;
};

//**MAIN */
int				main(int argc, char **argv);

//**PARSER */
void			parse_input(t_desk *desk, char **argv);

//**INIT */
void			data_init(t_desk *desk);

//**SIMULATION */
void			start_simulation(t_desk *desk);

//**QUEUE */
void			pop_waiting_list(t_queue *list);
void			priority_sorter(t_queue *list);

//**CODERS */
void			get_the_dongles(t_coder *coder);
void			work_in_progress(t_coder *coder);
void			free_the_dongles(t_coder *coder);

//**CODER-HELPERS */
bool			did_simulation_ended(t_desk *desk);
bool			is_coder_burnt_out(t_coder *coder);
void			wait_your_turn(t_coder *coder, t_dongle *dongle);
//void			lock_dongles_in_order(t_dongle *left, t_dongle *right);

//** UTILS */
void			error_exit(const char *msg, const char *func_name);
void			*safe_malloc(size_t bytes, const char *func_name);
long			get_current_time_in_ms(void);
void			print_status(t_coder *coder, char *status);
void			free_all(t_desk *desk);

#endif