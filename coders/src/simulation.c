#include "codex.h"

static void    *run_coder(void *argv)
{
    t_coder *coder;
    
    coder = (t_coder *)argv; // casteamos para que lo que entró se convierta en coder
    while (!did_simulation_ended(coder->desk)) //bucle infinito
    {
        // asignar los libres al coder
        assign_the_dongles(coder);
        
        if (did_simulation_ended(coder->desk))
            break;
        // trabajar y toda la vuelta
        work_in_progress(coder);
    }

    return(NULL);
}

static void *somebody_died(t_coder *coder)
{
    pthread_mutex_lock(&coder->desk->log_mutex);
    if (!coder->desk->end_simulation)
    {
        printf("%ld %d burned out\n", get_current_time_in_ms() - coder->desk->start_time, coder->id);
        coder->desk->end_simulation = true;
    }
    pthread_mutex_unlock(&coder->desk->log_mutex);
    return (NULL);
}

static void call_it_a_day(t_desk *desk, long coder_done)
{
    pthread_mutex_lock(&desk->log_mutex);
    desk->end_simulation = coder_done == desk->number_of_coders;
    pthread_mutex_unlock(&desk->log_mutex);
}

void    *monitor(void *argv)
{
    int         i;
    long        coder_done;
    t_desk      *desk;
    t_coder     *coder;

    desk = (t_desk *)argv;
    while (!did_simulation_ended(desk))
    {
        i = 0;
        coder_done = 0;
        call_it_a_day(desk, coder_done); // en caso de que requiera 0 compilaciones
        while (i < desk->number_of_coders)
        {
            coder = &desk->coders[i];
            if (is_coder_burnt_out(coder))
            {
                somebody_died(coder);
                return (NULL);
            }
            if (did_simulation_ended(desk))
                break;
            if (coder->compiler_counter >= desk->number_of_compiles_required)
                coder_done ++;
            i++;
        }
        if (did_simulation_ended(desk))
            return(NULL);
        call_it_a_day(desk, coder_done);
        usleep(500);
    }
    return(NULL);
}


void    start_simulation(t_desk *desk)
{
    int i;
    i = 0;
    if (desk->number_of_compiles_required == 0)
        return;
    desk->start_time = get_current_time_in_ms();
    
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