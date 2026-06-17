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

// no entiendo una mierda esa función, esto me explica gemini
//Paso 1: Reservar espaciostruct timeval t_time_val; crea un contenedor vacío en la memoria diseñado específicamente para guardar el tiempo con alta precisión.Paso 2: Consultar al reloj del sistemagettimeofday(&t_time_val, NULL); viaja al núcleo del sistema operativo, lee la hora exacta del reloj de la máquina y rellena el contenedor con dos datos: los segundos y los microsegundos actuales.Paso 3: Convertir los segundos((long)t_time_val.tv_sec * 1000) toma los segundos transcurridos desde 1970 y los multiplica por 1000 para transformarlos en milisegundos. El cambio a (long) evita que el número se vuelva demasiado grande y rompa el programa.Paso 4: Convertir la fracción de tiempo(t_time_val.tv_usec / 1000) toma los microsegundos restantes y los divide entre 1000 para convertirlos también en milisegundos.Paso 5: Fusionar y entregarEl return suma ambos resultados parciales para obtener una única cifra final en milisegundos y se la envía de vuelta a la variable que llamó a la función.