# Codexion
Codexion is a C programming project that challenges developers to solve problems related to concurrency and resource synchronization using POSIX threads and mutexes

Files organization:

main.c:
    main function, calls these functions:
    - parse_input(&desk, argv);
    - data_init(&desk);
    - start_codexion(&desk);
parser.c:
    Parse and validate the input and asign entered 8 values to t_desk

init.c:
    Initializes lasting values of t_desk, t_coder and t_dongle

simulation.c:
    Initializes the coders threads with the funcion run_coder(&coder)
    run_coder() calls:
    - a function to verify if the simulation can continue
    - assign_the_dongles(coder);
    - work_in_progress(coder);

queue.c:
    Contains all functions needed to organize the dongle_queue

coders.c:
    Contains all the task the coder thread must execute related to assign and free the dongles and also the compile, debbug and refactor.

utils.c:
    Contains:
    - safe_malloc()
    - get_current_time_in_ms()
    - error_exit()