#include "codex.h"

static void swap_list_items(t_queue *list, int index, int most_urgent)
{
    t_queue_item tmp;
    tmp = list->array[index];
    list->array[index] = list->array[most_urgent];
    list->array[most_urgent] = tmp;
}

// static void    push_waiting_list(t_queue *list, int coder_id, long long priority)
// {
//     int current_index;
//     int parent_index;
//     t_queue_item  new_item;
//     current_index = list->size;
//     new_item.coder_id = coder_id;
//     new_item.priority = priority;
//     list->array[current_index] = new_item;
//     list->size ++;
//     while(current_index > 0)
//     {
//         parent_index = (current_index - 1) / 2;
//         if (list->array[current_index].priority < list->array[parent_index].priority)
//         {
//             swap_list_items(list, current_index, parent_index);
//             current_index = parent_index;
//         }
//         else
//             break; // si ya llegamos al sitio, chau
//     }
// }

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
    priority_sorter(list, 0);
    return (item_ret); // le devuelve el id al flujo para que se le asigne un dongle al coder con este id, que por fin llego a la cima. 
}

void    priority_sorter(t_queue *list, int index)
{
    int most_urgent;
    int left_item;
    int right_item;

    most_urgent = index;
    left_item = (2 * index) + 1;
    right_item = (2 * index) + 2;

    if (left_item < list->size)
        if (list->array[left_item].priority <
            list->array[most_urgent].priority)
            most_urgent = left_item;
    if (right_item < list->size)
        if (list->array[right_item].priority <
            list->array[most_urgent].priority)
            most_urgent = right_item;
    if (most_urgent != index)
    {
        swap_list_items(list, index, most_urgent);
        priority_sorter(list, most_urgent);
    }
}