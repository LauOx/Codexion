pruebas:

comando valgrind:

 valgrind --leak-check=full --show-leak-kinds=all 

escenario ideal:

Comando: ./codexion 4 800 100 100 100 5 10 fifo
Explicación: Con 800 ms de vida, un cooldown bajísimo de 10 ms y orden de llegada básico (fifo), los 4 coders completarán sus 5 compilaciones de forma holgada y pacífica.

Comando: ./codexion 3 50 200 100 100 10 0 fifo
Explicación: Un coder necesita 200 ms para compilar, pero solo tiene 50 ms de vida inicial. Nadie llegará jamás a compilar. El monitor debe congelar el programa exactamente en el milisegundo 50.

Comando: ./codexion 3 600 50 50 50 4 200 fifo
Explicación: Aunque compilar solo toma 50 ms, el dongle se queda congelado durante 200 ms después de que alguien lo suelta. Esto va a generar un tapón masivo en la cola de dongles. Revisa que los coders esperen pacientemente sin consumir 100% de CPU.

Prueba A (Debería fallar/morir con FIFO):
./codexion 3 350 100 100 100 3 10 fifo
Prueba B (Debería salvarse/terminar con EDF):
./codexion 3 350 100 100 100 3 10 edf
Explicación: Al estar en el límite de tiempo (350 ms de vida frente a 300 ms de ciclo), si un coder con mucha prisa por compilar (porque le queda poca vida) se queda atrapado detrás de otro que acaba de empezar en la cola, en FIFO morirá esperando. En EDF, tu función priority_sorter detectará que el rezagado tiene un deadline más urgente, lo pondrá al principio de la cola (array[0]), compilará a tiempo y todos sobrevivirán.

Objetivo instantáneo: ./codexion 3 600 100 100 100 0 10 edf
Explicación: Al pedir 0 compilaciones requeridas, tu programa debe parsear con éxito los 8 parámetros y cerrarse en el milisegundo 0 de forma limpia, sin inicializar hilos o cerrándolos de inmediato sin leaks.