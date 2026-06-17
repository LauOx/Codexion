#include "codex.h"

static int valid_input(const char *str)
{
    int num;
    num = atoi(str);
    if (num <= 0)
        error_exit("Input is not a valid positive integrer > 0", __func__);
    return (num);
}

static int valid_cooldown(const char *str)
{
    int num;
    if (strcmp(str, "0") == 0)
        return (0);
    num = atoi(str);
    if (num <= 0)
        error_exit("'time_to_cooldown' is not a valid positive integrer", __func__);
    return (num);
}
static int  ft_strlen(char *str)
{
    int i;
    i = 0;
    while(str[i])
        i++;
    return (i);
}

void    parse_input(t_desk *desk, char **argv)
{
    int i;
    char *scheduler_str;
    printf("estamos dentro de parser\n");
    desk->number_of_coders = valid_input(argv[1]);
    desk->time_to_burnout = valid_input(argv[2]);
    desk->time_to_compile = valid_input(argv[3]);
    desk->time_to_debug = valid_input(argv[4]);
    desk->time_to_refactor = valid_input(argv[5]);
    desk->number_of_compiles_required = valid_input(argv[6]);
    desk->dongle_cooldown = valid_cooldown(argv[7]);
    if(strcmp(argv[8], "fifo") == 0 || strcmp(argv[8], "edf") == 0)
    {
        scheduler_str = safe_malloc(sizeof(char) * ft_strlen(argv[8]) + 1, __func__);
        i = 0;
        while(argv[8][i])
        {
            scheduler_str[i] = argv[8][i];
            i++;
        }
        scheduler_str[i] = 0;
        desk->scheduler = scheduler_str;
    }
    else
         error_exit("Scheduler must be 'fifo' or 'edf'", __func__);
}