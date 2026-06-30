*Este proyecto ha sido creado como parte del currículo de 42 por lospina-.*

# Codexion

## Descripción
**Codexion** es un proyecto de simulación concurrente diseñado para resolver variaciones del problema clasico de *Dining Philosophers* aplicado a el entorno de desarrollo de software moderno.

La simulación consiste en un set de `coders` (hilos) que necesitan ejecutar tareas (Compile, Debug and Refactor). Para compilar, el `coder` debe contar con 2 dongles (recursos compartidos). El reto principal de este proyecto es gestionar la competencia por estos recursos basandose en una politica de arbitraje (**FIFO** Y **EDF**), evitar probelmas de sincronización y diseñar un hilo `monitor` que puenda detener la simulación ante el agotamiento (`burnout`).

---

## Instrucciones

### Compilación
El proyecto incluye un `makefile`. Para compilar el ejecutable usa el siguiente comando:

```bash
make
```
Otros comandos:

Borrar todos los objetos (.o)
```bash
make clean
```

Borrar todos los objetos (.o) y los ejecutables
```bash
make fclean
```

Compila el proyecto desde cero
```bash
make re
```

### Ejecución
El programa espera 8 argumentos obligatorios:

- number_of_coders: Cantidad de coders en la simulación
- time_to_burnout: Si un coder pasa <time_to_burnout> tiempo sin compilar, desde su ultima compilación, se agota (en ms).
- time_to_compile: tiempo que tarda un coder en hacer una compilación (en ms).
- time_to_debug: tiempo que tarda un coder en hacer debug: tiempo que tarda un coder en hacer debug (en ms).
- time_to_refactor: tiempo que tarda un coder en hacer refactor (en ms).
- number_of_compiles_required>: cada coder debe compilar este numero de veces para que el programa se ejecute exitosamente (entero positivo)
- dongle_cooldown: tiempo que tarda el recurso (dongle) en enfriarse antes de poder usarse de nuevo (en ms).
- scheduler: Define la gestión de la prioridad para asignar el recurso (dongle). Puede ser *fifo* (first in, first out) o *edf* (earliest deadline first).

### Pruebas de uso

Comando valgrind:
```bash
 valgrind --leak-check=full --show-leak-kinds=all 
```

Escenario ideal:

```bash
./codexion 4 800 100 100 100 5 10 fifo
```

**Explicación:** Con 800 ms de vida, un cooldown bajísimo de 10 ms y orden de llegada básico (fifo), los 4 coders completarán sus 5 compilaciones de forma holgada y pacífica.

```bash
./codexion 3 50 200 100 100 10 0 fifo
```

**Explicación:** *Burnout* Un coder necesita 200 ms para compilar, pero solo tiene 50 ms de vida inicial. Nadie llegará jamás a compilar. El monitor debe detener el programa alrededor del milisegundo 50.

```bash
./codexion 3 600 50 50 50 4 200 fifo
```

**Explicación:** *funciona al limite* Aunque compilar solo toma 50 ms, el dongle se queda congelado durante 200 ms después de que alguien lo suelta. Esto va a generar un tapón masivo en la cola de dongles. Los coders deben esperar pacientemente sin consumir 100% de CPU.

Prueba A (Debería fallar/morir con FIFO):
```bash
./codexion 3 160 40 10 10 2 0 fifo
```

Prueba B (Debería salvarse/terminar con EDF):
```bash
./codexion 3 160 40 10 10 2 0 edf
```

**Explicación:** Al estar en el límite de tiempo (160 ms de vida frente a 40 ms de ciclo), si un coder con mucha prisa por compilar (porque le queda poca vida) se queda atrapado detrás de otro que acaba de empezar en la cola, en FIFO morirá esperando. En EDF, la función priority_sorter detectará que el rezagado tiene un deadline más urgente, lo pondrá al principio de la cola (array[0]), compilará a tiempo y todos sobrevivirán.

```bash
./codexion 3 600 100 100 100 0 10 edf
```

**Explicación:** Al pedir 0 compilaciones requeridas, el programa debe parsear con éxito los 8 parámetros y cerrarse en el milisegundo 0 de forma limpia, sin inicializar hilos o cerrándolos de inmediato sin leaks.

## Recursos
Estos fueron algunos de los recursos usados para entener los conceptos aplicados en el proyecto:
- Video de Youtube *The dining philosophers in C: threads, race conditions and deadlocks* del usuario Oceano https://www.youtube.com/watch?v=zOpzGHwJ3MU
- Artículo de Medium *The dining philosophers* del usuario oceanO https://medium.com/@jalal92/the-dining-philosophers-7157cc05315
- Articulo de Medium *Exploring the Practical Use of Threads with Starbucks* del usuario oceanO https://medium.com/@jalal92/lets-discuss-threads-grab-a-coffee-ad4d4ebf7181
- Video de Youtube *Introduction To Threads (pthreads) | C Programming Tutorial* del usuario Portfolio Courses https://www.youtube.com/watch?v=ldJ8WGZVXZk&t=461s

## Uso de la IA
Durante el desarrollo del proyecto se han usado algunos asistentes gratuitos de IA para las siguientes tareas:
- Analisis del enunciado y planteamiento del paso a paso para desarrollar el proyecto.
- Clarificación de conceptos.
- Ayuda para entender bugs y sugerencias para resolverlos.
- Redacción y traducción al inglés del archivo README.md


*English version*

*This project has been created as part of the 42 curriculum by lospina-.*

# Codexion
## Description
*Codexion* is a concurrent simulation project designed to solve variations of the classic Dining Philosophers problem, adapted to a modern software development environment.

The simulation consists of a set of `coders` (threads) that need to execute tasks (Compile, Debug, and Refactor). To compile, each `coder` must have 2 `dongles` (shared resources). The main challenge of this project is managing competition for these resources based on an arbitration policy (*FIFO* and *EDF*), avoiding synchronization problems, and designing a monitor thread capable of stopping the simulation in case of exhaustion (`burnout`).

## Instructions
### Compilation
The project includes a Makefile. To build the executable, use the following command:
```bash
make
```
Other commands:

Remove all object files (.o)
```bash
make clean
```
Remove all object files (.o) and executables
```bash
make fclean
```
Build the project from scratch
```bash
make re
```
### Execution
The program expects 8 mandatory arguments:
- *number_of_coders:* Number of coders in the simulation
- *time_to_burnout:* If a coder goes more than <time_to_burnout> time without compiling since  - their last compilation, they burn out (in ms).
- *time_to_compile:* time a coder takes to perform a compilation (in ms).
- *time_to_debug:* time a coder takes to debug: time a coder takes to debug (in ms).
- *time_to_refactor:* time a coder takes to refactor (in ms).
- *number_of_compiles_required:* each coder must compile this number of times for the program to successfully complete (positive integer)
- *dongle_cooldown:* time it takes for the resource (dongle) to cool down before it can be used again (in ms).
- *scheduler:* Defines the priority management for assigning the resource (dongle). It can be fifo (first in, first out) or edf (earliest deadline first).

### Usage tests
Valgrind command:
```bash
valgrind --leak-check=full --show-leak-kinds=all
```

Ideal scenario:
```bash
./codexion 4 800 100 100 100 5 10 fifo
```
Explanation: With 800 ms of lifetime, a very low cooldown of 10 ms, and basic arrival order (FIFO), the 4 coders will complete their 5 compilations smoothly and peacefully.

```bash
./codexion 3 50 200 100 100 10 0 fifo
```
Explanation: Burnout A coder needs 200 ms to compile, but only has 50 ms of initial life. No one will ever manage to compile. The monitor must stop the program exactly at millisecond 51.

```bash
./codexion 3 600 50 50 50 4 200 fifo
```
Explanation: borderline case Although compilation only takes 50 ms, the dongle stays frozen for 200 ms after being released. This will create a massive bottleneck in the dongle queue. Coders must wait patiently without consuming 100% CPU.

Test A (Should fail/die with FIFO):
```bash
./codexion 3 350 100 100 100 3 10 fifo
```
Test B (Should survive/finish with EDF):
```bash
./codexion 3 350 100 100 100 3 10 edf
```
Explanation: Being at the time limit (350 ms lifetime vs 300 ms cycle), if a coder in a hurry to compile (because they have little life left) gets stuck behind another one that just started in the queue, with FIFO they will die waiting. With EDF, the priority_sorter function detects that the lagging coder has a more urgent deadline, moves it to the front of the queue (array[0]), compiles in time, and everyone survives.

```bash
./codexion 3 600 100 100 100 0 10 edf
```
Explanation: When requesting 0 required compilations, the program must successfully parse all 8 parameters and exit cleanly at millisecond 0, without initializing threads or shutting them down immediately without leaks.

## Resources
These were some of the resources used to understand the concepts applied in this project:
- YouTube video The dining philosophers in C: threads, race conditions and deadlocks by Oceano https://www.youtube.com/watch?v=zOpzGHwJ3MU
- Medium article The dining philosophers by oceanO https://medium.com/@jalal92/the-dining-philosophers-7157cc05315
- Medium article Exploring the Practical Use of Threads with Starbucks by oceanO https://medium.com/@jalal92/lets-discuss-threads-grab-a-coffee-ad4d4ebf7181
- YouTube video Introduction To Threads (pthreads) | C Programming Tutorial by Portfolio Courses https://www.youtube.com/watch?v=ldJ8WGZVXZk&t=461s

## Use of AI
During the development of this project, some free AI assistants were used for the following tasks:
- Analysis of the subject and step-by-step planning of the project implementation.
- Clarification of concepts.
- Help understanding bugs and suggestions for fixing them.
- Writing and translating the README.md into English

