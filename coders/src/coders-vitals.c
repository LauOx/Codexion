#include "codex.h"

bool is_coder_burnt_out(t_coder *coder)
{
    return ((get_current_time_in_ms() - coder->last_comp_time)
        > coder->desk->time_to_burnout);
}

bool did_simulation_ended(t_desk *desk)
{
    bool stop;

    pthread_mutex_lock(&desk->log_mutex);
    stop = desk->end_simulation;
    pthread_mutex_unlock(&desk->log_mutex);
    
    return (stop);
}