#include "codex.h"

static void    *run_coder(void *argv)
{
    t_coder *coder;
    
    coder = (t_coder *)argv; // casteamos para que lo que entró se convierta en coder
    while (!coder->desk->end_simulation) //bucle infinito
    {
        // asignar los libres al coder
        assign_the_dongles(coder);
        // trabajar y toda la vuelta
        work_in_progress(coder);
    }

    return(NULL);
}

static void *somebody_died(t_coder *coder)
{
    print_status(coder, "burned out");
    coder->desk->end_simulation = true;
    return (NULL);
}

static bool call_it_a_day(t_desk *desk, int compilation_done)
{
    return (compilation_done == desk->number_of_compiles_required);
}

void    *monitor(void *argv)
{
    int         i;
    int         compilation_done;
    t_desk      *desk;
    t_coder     *coder;

    desk = (t_desk *)argv;
    while (!desk->end_simulation)
    {
        i = 0;
        compilation_done = 0;
        while (i < desk->number_of_coders)
        {
            coder = &desk->coders[i];
            if ((get_current_time_in_ms() - coder->last_comp_time) > desk->time_to_burnout)
            {
                somebody_died(coder);
                return (NULL);
            }
            if (coder->compiler_counter >= desk->number_of_compiles_required)
                compilation_done ++;
            i++;
        }
        desk->end_simulation = call_it_a_day(desk, compilation_done);
        usleep(500);
    }
    return(NULL);
}


void    start_simulation(t_desk *desk)
{
    int i;
    i = 0;
    desk->start_time = get_current_time_in_ms();
    ///** PRUEBA VARIABLES */
    printf("empezamos a las %ld\n", desk->start_time);
    //**PRUEBA VARIABLES */
    
    // inicio de timepo de ultima compilación
    while (i < desk->number_of_coders)
    {
        desk->coders[i].last_comp_time = desk->start_time;
        i++;
    }

    // inicio de hilos de coder
    i = 0;
    while (i < desk->number_of_coders)
    {
        pthread_create(&desk->coders[i].thread_id, NULL, run_coder, &desk->coders[i]);
        i++;
    }

    // inicio de hilo monitor
     pthread_create(&desk->start_simulation, NULL, monitor, desk);
    
    // join de los hilos coders
    i = 0;
    while (i < desk->number_of_coders)
    {
        pthread_join(desk->coders[i].thread_id, NULL);
        i++;
    }
    pthread_join(desk->start_simulation, NULL);
}