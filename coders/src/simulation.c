#include "codex.h"

static void    *run_coder(void *argv)
{
    t_coder *coder;
    
    coder = (t_coder *)argv; // casteamos para que lo que entró se convierta en coder
    while (1) //bucle infinito
    {
        if(somebody_died_or_we_are_done(coder->desk)) // pendiente esta funcion avisa si hay que parar.
            break;
        // asignar los libres al coder
        assign_the_dongles(coder);
        // trabajar y toda la vuelta
        work_in_progress(coder);
    }

    return(NULL);
}


void    start_codexion(t_desk *desk)
{
    int i;
    i = 0;
    while (i < desk->number_of_coders)
    {
        pthread_create(&desk->coders[i].thread_id, NULL, run_coder, &desk->coders[i]);
        i++;
    }
    i = 0;
    while (i < desk->number_of_coders)
    {
        pthread_join(desk->coders[i].thread_id, NULL);
        i++;
    }
}

void    monitor_thread()
{
    
}