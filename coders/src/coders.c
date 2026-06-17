#include "codex.h"

static void    add_to_queue(t_desk *desk, t_queue *list, t_coder coder)
{
    t_queue_item  new_item;
    int queue_index;
    new_item.coder_id = coder.id;
    if (strcmp(desk->scheduler, "fifo") == 0)
        new_item.priority = (long long)time(NULL);
    else{
        new_item.priority = (coder.last_comp_time + desk->time_to_burnout);
    }
    queue_index = list->size;
    list->array[queue_index] = new_item;
    list->size ++;
    priority_sorter(list, 0);
}

static void wait_for_the_dongle(t_coder *coder)
{
    t_dongle *left;
    t_dongle *right;
    long timestamp;

    left = &coder->left_dongle;
    right = &coder->right_dongle;
    add_to_waiting_list(coder->desk, &left->dongle_queue, *coder);
    add_to_waiting_list(coder->desk, &right->dongle_queue, *coder);

    timestamp = get_current_time_in_ms();
    while ((left->dongle_queue.array[0].coder_id != coder->id)
            || (timestamp < left->free_at))
    {
        pthread_cond_wait(&left->cond, &left->mutex);
        timestamp = get_current_time_in_ms();
    }
    
    while ((right->dongle_queue.array[0].coder_id != coder->id)
            || (timestamp < right->free_at))
    {
        pthread_cond_wait(&right->cond, &right->mutex);
        timestamp = get_current_time_in_ms();
    }
}

static void free_the_dongles(t_coder *coder)
{
    long timestamp;

    timestamp = get_current_time_in_ms();
    
    //left_dongle
    coder->left_dongle->free_at = timestamp + coder->left_dongle->cooldown_wait;
    coder->left_dongle->holding_coder_id = -1;
    pthread_mutex_unlock(&coder->left_dongle->mutex); //desbloquea el recurso
    pthread_cond_broadcast(&coder->left_dongle->cond); // despierta a los coders que estan en fila para que revisen si son el siguiente
    
    //right_dongle
    coder->right_dongle->free_at = timestamp + coder->right_dongle->cooldown_wait;
    coder->right_dongle->holding_coder_id = -1; 
    pthread_mutex_unlock(&coder->right_dongle->mutex);
    pthread_cond_broadcast(&coder->right_dongle->cond);
}

void    assign_the_dongles(t_coder *coder)
{
    t_dongle *left;
    t_dongle *right;
    long    timestamp;
    
    left = &coder->left_dongle;
    right = &coder->right_dongle;

    wait_for_the_dongle(coder);

    // marcar el dongle como ocupado por coder
    left->holding_coder_id = coder->id;
    right->holding_coder_id = coder->id;

    // saca al coder de la lista de espera
    pop_waiting_list(&left->dongle_queue);
    pop_waiting_list(&right->dongle_queue);
  
    timestamp = get_current_time_in_ms();
    pthread_mutex_lock(&coder->desk->log_mutex);
    print("%ld %d has taken a dongle\n", timestamp, coder->id);
    pthread_mutex_unlock(&coder->desk->log_mutex);

    timestamp = get_current_time_in_ms();
    pthread_mutex_lock(&coder->desk->log_mutex);
    print("%ld %d has taken a dongle\n", timestamp, coder->id);
    pthread_mutex_unlock(&coder->desk->log_mutex);
}

void    work_in_progress(t_coder *coder)
{
    long    timestamp;

    // compile
    timestamp = get_current_time_in_ms(); 
    pthread_mutex_lock(&coder->desk->log_mutex);
    printf("%ld %d is compiling\n", timestamp, coder->id);
    pthread_mutex_unlock(&coder->desk->log_mutex);
    usleep(coder->desk->time_to_compile * 1000);
    free_the_dongles(coder);
    coder->compiler_counter ++;
    
    //debug
    timestamp = get_current_time_in_ms();
    pthread_mutex_lock(&coder->desk->log_mutex);
    printf("%ld %d is debugging\n", timestamp, coder->id);
    pthread_mutex_unlock(&coder->desk->log_mutex);
    usleep(coder->desk->time_to_debug * 1000);

    //refactor
    timestamp = get_current_time_in_ms();
    pthread_mutex_lock(&coder->desk->log_mutex);
    printf("%ld %d is refactoring\n", timestamp, coder->id);
    pthread_mutex_unlock(&coder->desk->log_mutex);
    usleep(coder->desk->time_to_refactor * 1000);

}