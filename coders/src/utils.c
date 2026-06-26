#include "codex.h"

void    error_exit(const char *msg, const char *func_name)
{
    fprintf(stderr, "Found error in function: '%s': %s\n", func_name, msg);
    exit(1);
}

void    *safe_malloc(size_t bytes, const char *func_name)
{
    void    *ret;
    ret = malloc(bytes);
    if(!ret)
        error_exit("Malloc allocation failed", func_name);
    return (ret);
}

long    get_current_time_in_ms(void)
{
    struct timeval t_time_val;
    gettimeofday(&t_time_val, NULL);
    return((long)t_time_val.tv_sec * 1000 +
    (t_time_val.tv_usec / 1000)); 
}

void    print_status(t_coder *coder, char *status)
{
    long    timestamp;
    pthread_mutex_lock(&coder->desk->log_mutex);
    timestamp = get_current_time_in_ms() - coder->desk->start_time;
    printf("%ld %d %s\n", timestamp, coder->id, status);
    pthread_mutex_unlock(&coder->desk->log_mutex);
}

void    free_all(t_desk *desk)
{
    int i;
    i = 0;
    while(desk->number_of_coders > i)
    {
        free(desk->dongles[i].dongle_queue.array);
        i++;
    }
    free(desk->coders);
    free(desk->dongles);
    free(desk->scheduler);

}