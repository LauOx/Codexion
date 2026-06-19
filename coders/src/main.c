#include "codex.h"

int main(int argc, char **argv)
{
    t_desk desk;

    if (argc == 9)
    {
        //errors checking
        parse_input(&desk, argv);

        //**PRUEBA TODAS LAS VARIABLES */
        printf("Variables:\n");
        printf("number_of_coders: %ld\n", desk.number_of_coders);
        printf("time_to_burnout: %lld\n", desk.time_to_burnout);
        printf("time_to_compile: %lld\n", desk.time_to_compile);
        printf("time_to_debug: %lld\n", desk.time_to_debug);
        printf("time_to_refactor: %lld\n", desk.time_to_refactor);
        printf("compiles_required: %ld\n", desk.number_of_compiles_required);
        printf("dongle_cooldown: %lld\n", desk.dongle_cooldown);

        //**PRUEBA TODAS LAS VARIABLES */

        // initialize co-working desk
        data_init(&desk);
        // start hub simultion
        start_simulation(&desk);
        //no leaks, when the coders end the compilation or burnt out
        //clean(&desk);
    }
    else{
        // function print error and exit
        error_exit("Wrong input, must be 8 positive integrers", __func__);
    }
    return (0);
}