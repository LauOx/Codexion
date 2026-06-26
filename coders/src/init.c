#include "codex.h"

static  void coder_init(t_desk *desk)
{
    int i;
    i = 0;
    while(i < desk->number_of_coders)
    {
        desk->coders[i].id = i + 1;
        desk->coders[i].compiler_counter = 0;
        desk->coders[i].comp_done = false;
        desk->coders[i].last_comp_time = 0;
        desk->coders[i].desk = desk;
        // asignación circular de dongles
        // como dongles en desk son punteros, se usa & para guardar la dirección del dongle[i] en el array
        desk->coders[i].left_dongle = &desk->dongles[i];
        desk->coders[i].right_dongle = &desk->dongles[(i + 1) % desk->number_of_coders];
        i++;
    }
}

// inicializa las variables del dongle
static void    dongles_init(t_desk *desk)
{
    int i;
    i = 0;
    while(i < desk->number_of_coders)
    {
        desk->dongles[i].dongle_id = i; // asigno un id al dongle desde el array de dongles de desk
        desk->dongles[i].holding_coder_id = -1;
        desk->dongles[i].cooldown_wait = 0;
        desk->dongles[i].free_at = 0;
        pthread_mutex_init(&desk->dongles[i].mutex, NULL);
        pthread_cond_init(&desk->dongles[i].cond, NULL);
        // wait_queue init
        desk->dongles[i].dongle_queue.array = safe_malloc(
            sizeof(t_queue_item) * desk->number_of_coders, __func__);
        desk->dongles[i].dongle_queue.size = 0;
        i++;
    }
}
// inicializa las otras variables del co-working
static void    desk_init(t_desk *desk)
{
    desk->start_simulation = 0;
    desk->end_simulation = false;
    desk->coders = safe_malloc(sizeof(t_coder) * desk->number_of_coders, __func__);
    desk->dongles = safe_malloc(sizeof(t_dongle) * desk->number_of_coders, __func__);
    pthread_mutex_init(&desk->log_mutex, NULL);
    pthread_mutex_init(&desk->sim_mutex, NULL);
}

void    data_init(t_desk *desk)
{
    desk_init(desk);
    dongles_init(desk);
    coder_init(desk);
}