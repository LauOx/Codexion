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

    left = coder->left_dongle;
    right = coder->right_dongle;
    
    pthread_mutex_lock(&left->mutex);
    add_to_queue(coder->desk, &left->dongle_queue, *coder);
    timestamp = get_current_time_in_ms();
    while ((left->dongle_queue.array[0].coder_id != coder->id)
            || (timestamp < left->free_at))
    {
        pthread_cond_wait(&left->cond, &left->mutex);
        timestamp = get_current_time_in_ms();
    }
    left->holding_coder_id = coder->id;
    pop_waiting_list(&left->dongle_queue);
    pthread_mutex_unlock(&left->mutex);
    
    pthread_mutex_lock(&right->mutex);
    add_to_queue(coder->desk, &right->dongle_queue, *coder);
    while ((right->dongle_queue.array[0].coder_id != coder->id)
            || (timestamp < right->free_at))
    {
        pthread_cond_wait(&right->cond, &right->mutex);
        timestamp = get_current_time_in_ms();
    }
    right->holding_coder_id = coder->id;
    pop_waiting_list(&right->dongle_queue);
    pthread_mutex_unlock(&right->mutex);
}

static void free_the_dongles(t_coder *coder)
{
    long timestamp;

    timestamp = get_current_time_in_ms();
    
    //left_dongle
    pthread_mutex_lock(&coder->left_dongle->mutex);
    coder->left_dongle->free_at = timestamp + coder->left_dongle->cooldown_wait;
    coder->left_dongle->holding_coder_id = -1;
    pthread_mutex_unlock(&coder->left_dongle->mutex); //desbloquea el recurso
    pthread_cond_broadcast(&coder->left_dongle->cond); // despierta a los coders que estan en fila para que revisen si son el siguiente
    
    //right_dongle
    pthread_mutex_lock(&coder->right_dongle->mutex);
    coder->right_dongle->free_at = timestamp + coder->right_dongle->cooldown_wait;
    coder->right_dongle->holding_coder_id = -1; 
    pthread_mutex_unlock(&coder->right_dongle->mutex);
    pthread_cond_broadcast(&coder->right_dongle->cond);
}

void    assign_the_dongles(t_coder *coder)
{
    wait_for_the_dongle(coder);
  
    print_status(coder, "has taken a dongle");

    print_status(coder, "has taken a dongle");
}

void    work_in_progress(t_coder *coder)
{
    // compile
    print_status(coder, "is compiling");
    usleep(coder->desk->time_to_compile * 1000);
    free_the_dongles(coder);
    coder->compiler_counter ++;
    
    //debug
    print_status(coder, "is debbuging");
    usleep(coder->desk->time_to_debug * 1000);

    //refactor
    print_status(coder, "is refactoring");
    usleep(coder->desk->time_to_refactor * 1000);

}