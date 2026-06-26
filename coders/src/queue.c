#include "codex.h"

t_queue_item    pop_waiting_list(t_queue *list)
{
    int last_item;
    t_queue_item  item_ret;

    if (list->size == 0)
    {
        item_ret.coder_id = -1;
        return (item_ret);
    }
    last_item = list->size - 1;
    item_ret = list->array[0];
    list->array[0] = list->array[last_item];
    list->size--;
    priority_sorter(list);
    return (item_ret); // le devuelve el id al flujo para que se le asigne un dongle al coder con este id, que por fin llego a la cima. 
}

void    priority_sorter(t_queue *list)
{
    t_queue_item tmp;
    int item_zero;
    int item_one;

    if (list->size == 2)
    {
        item_zero = list->array[0].priority;
        item_one = list->array[1].priority;
        if (item_zero < item_one)
        {
            tmp = list->array[0];
            list->array[0] = list->array[1];
            list->array[1] = tmp;
        }
    }
    else
        return ;
}