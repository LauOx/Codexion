#include "codex.h"

int main(int argc, char **argv)
{
    t_desk desk;
    printf("argc = %d\n", argc);
    if (argc == 9)
    {
        //errors checking
        parse_input(&desk, argv);
        // initialize co-working desk
        data_init(&desk);
        // start hub simultion
        start_codexion(&desk);
        //no leaks, when the coders end the compilation or burnt out
        //clean(&desk);
    }
    else{
        // function print error and exit
        error_exit("Wrong input, must be 8 positive integrers", __func__);
    }
    return (0);
}