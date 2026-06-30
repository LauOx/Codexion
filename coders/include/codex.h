/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospina- <lospina-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 13:28:37 by lospina-          #+#    #+#             */
/*   Updated: 2026/06/30 23:59:27 by lospina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CODEX_H
# define CODEX_H

//**LIBRARIES */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // usleep
#include <stdbool.h> // bool, true, false
#include <pthread.h> //mutex, threads, cond variables
#include <sys/time.h> // gettimeofday
#include <limits.h> // INT_MAX
#include <string.h> // strcmp 

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
	t_queue_item	*array;
	int				size;
};

//**  DONGLES*/
struct s_dongle
{
	pthread_mutex_t	mutex; // Protege el estado de este dongle específico
    pthread_cond_t	cond; //para que los coders esperen por este dongle
    int             dongle_id; 
    int             cooldown_wait; // está en periodo de cooldown
    long            free_at; // timestamp en el que termina el cooldown
    t_queue         dongle_queue; // cola de prioridad fifo o edf (cada dongle tiene su cola de espera)
    int             holding_coder_id; // quien lo tiene ahora, -1 si está libre
};

//** CODERS */
struct s_coder
{
    int         id;
    long        compiler_counter;
    bool        comp_done;
    long        last_comp_time;
    t_dongle    *left_dongle;
    t_dongle    *right_dongle;
    pthread_t   thread_id; // coder thread
    t_desk      *desk;
};

//**CO-WORKING DESK */
struct s_desk
{
    long            number_of_coders;
    long long       time_to_burnout;
    long long       time_to_compile;
    long long       time_to_debug;
    long long       time_to_refactor;
    long            number_of_compiles_required;
    long long       dongle_cooldown;
    char            *scheduler;
    unsigned long   start_simulation;
    bool            end_simulation; // when coders burnt out or compile
    long            start_time;
    pthread_mutex_t log_mutex; // para que los printf no se mezclen
    pthread_mutex_t sim_mutex; // para leer y escribir end_simulation de forma segura
    t_dongle        *dongles; // punteros.. por qué? no se aun
    t_coder         *coders;
};

//**MAIN */
int main(int argc, char **argv);

//**PARSER */
void parse_input(t_desk *desk, char **argv);

//**INIT */
void    data_init(t_desk *desk);

//**SIMULATION */
void    start_simulation(t_desk *desk);

//**QUEUE */
t_queue_item    pop_waiting_list(t_queue *list);
void    priority_sorter(t_queue *list);

//**CODERS */
void    assign_the_dongles(t_coder *coder);
void    work_in_progress(t_coder *coder);
void	free_the_dongles(t_coder *coder);

//**CODERS-VITALS */
bool    did_simulation_ended(t_desk *desk);
bool    is_coder_burnt_out(t_coder *coder);
void	wait_your_turn(t_coder *coder, t_dongle *dongle);
void	lock_dongles_in_order(t_dongle *left, t_dongle *right);

//** UTILS */
void    error_exit(const char *msg, const char *func_name);
void    *safe_malloc(size_t bytes, const char *func_name);
long    get_current_time_in_ms(void);
void    print_status(t_coder *coder, char *status);
void    free_all(t_desk *desk);



#endif