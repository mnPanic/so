# Procesos y API del SO

## Temas centrales de la anterior

- Solucionar **concurrencia** y **contención**

Tiene dos responsabilidades principales, le toca estar entre medio del hardware
y del resto del software (específico)

1. visión del propietario: Proteger al hardware para que no se maluse, protege,
   le pone límites al programador.
2. Es una API de programación, hace que los recursos del hardware se pueden
   usar de forma sencilla.

El SO **arbitra** y **administra** los recursos del hardware, ya que son
escasos, y por eso se genera la *contención*, la cual debe estar resuelta
de manera eficiente y correcta.

## Proceso

El punto central de la clase es el **proceso**.

Para lidiar con el concepto de proceso, el SO crea una serie de abstracciones,
parte de la API del SO.

- Programa: Secuencia de pasos escritas en algún lenguaje.
- Eventualmente se convierte en asm.
- Cuando ese programa se pone a ejecutar lo llamamos **proceso**.

A cada uno se le asigna un identificador único, el *pid* (*process id*)

### Memoria

Un programa en ejecución tiene que tener asignado si o si un cacho de memoria.

- area de *texto*: código de máquina del programa
- area de *datos*: donde se almacena el heap
- stack del proceso (donde se almacenan las variables locales)

## Acciones que puede realizar un proceso

- Terminar.
- Lanzar un proceso hijo (`system()`, `fork()`, `exec()`)
- Ejecutar en la CPU: Hacer que sus instrucciones sean ejecutadas por el
  procesador, lo cual es central, para esto hacemos el programa.
  - Hacer una *system call* (*syscall*, llamada al sistema). No es más que
    una llamada a una función, pero no una cualquiera, sino una que brinda el
    sistema operativo. (es por eso que es una API de programación)
- Realizar una entrada / salida (E/S) a los dispositivos: hablar con el hardware
  (por lo general a través de una *syscall*)

### Terminar

Para terminar, ejecuta una syscall llamada `exit()`.

Con esto le indica al SO que puede liberar todos los recursos asignados al
proceso (por ej. memoria).

Además, indica su *status de terminación* (usualmente un código numérico), que
quien invocó el proceso (el padre) luego interpreta.
Por lo general, 0 es OK y > 1 con código de error.

### Arbol de procesos (lanzar un proceso hijo)

Todos los procesos tienen un padre, se organizan jerárquicamente como un árbol.

Cuando el SO comienza pone a poner cierto proceso (se suele llamar init, root)
es el que se encarga de ir desencadenando el resto de los procesos.

Eso se hace a través de la syscall `fork()`, una llamada al sistema que crea un
proceso exactamente igual al actual. La idea es que hay un camino que se
*bifurca*. El resultado de invocar a fork es el pid del proceso hijo, que es
una copia exacta del padre.

Fork duplica un proceso, y ambos procesos se quedan en el estado de acabo de
invocar a la función fork, pero a ambos les retorna cosas diferentes, a uno 0
(al hijo) y al otro un pid (al padre).

El típico código es

    pid = fork()
    if pid == 0 then
        // SOY EL HIJO
    else
        // SOY EL PADRE, y mi hijo tiene el process_id pid.

El PC quedaría en ambos procesos en la linea del `if`.

Ambos son el mismo programa, pero son *procesos* distintos.

El padre puede decidir suspenderse hasta que termine el hijo,
ejecutando `wait()`. Y cuando este termina, retorna el código de retorno del
`exit()` del hijo.

Y por ende `fork()` + `wait()` = `system()`

#### Lanzar el programa desde el shell

- Encuentra en que archivo dentro del file system se encuentra el código
- El shell hace un `fork()`
- Si es 0, es el proceso hijo, es una copia del shell.
  - Llama a la función `exec()`, que toma un archivo del disco y lo carga en
    memoria, reemplazando el código del proceso actual.
    Al exec le pasás el path de un programa, y lo carga en memoria, reemplazando
    el código que se está ejecutando por el que figura en el archivo que
    le pasás.
- Si no es 0, entonces es el padre, y teri

El código podría ser

    pid = fork()
    if pid == 0:
        exec(path que tipeo el usuario)
    else:
        wait(pid)

[imagen diapo 8]

#### pstree

[foto]

### Ejecutar en la CPU

Una vez que un proceso se esta ejecutando

- Hacer operaciones entre registros y posiciones de memoria
- I/O
- Llamadas al sistema (syscalls)

Por cuanto tiempo lo dejo ejecutar? (solo un proceso a la vez puede estar en
la CPU). Políticas:

- **Hasta que termina**: el proceso se queda con toda la computadora, no es
  lo mejor. A parte podría ser un while true y no termina nunca.
!! central !!
- Un **ratito**: se le llama *quantum*. Ver cual es la duración de este
  quantum es central.
  Por ej. 1/60 s. Entonces cada proceso se ejecuta durante esa cantidad de
  tiempo.
  En general no se especifica en segundos (aunque se traduzca) sino que
  **se mide en ticks de procesador (ciclos).**
  El SO entonces va haciendo round robin entre los procesos.
  Es tan chico contra la percepción humana el quantum que parece que se están
  ejecutando al mismo tiempo, cuando en realidad no es así.

Los SOs modernos hacen *preemtion*: (aka se ponen la gorra) procedimiento por el
cual un SO cuando se acaba su quantum lo saca de ejecución.

Preguntas

- Quien y como decide a quien le toca
- Que significa hacer que se ejecute otro proceso?

!! central !!

Aparece un modulo escencial para el SO; el ***scheduler*** o *planificador*.
Esto es lo que hace la diferencia entre la eficiencia del sistema operativo.
(se aborda la clase que viene)

#### Context switch

!! central !!

Que significa que le toque a otro? Para cambiar un programa que se ejecuta
en la CPU, y que cuando vuelva todo esté como estaba, debemos hacer un
**cambio de contexto** o ***context switch***

- Guardar los registros
- Guardar el IP (instruction pointer)
- Si es un programa nuevo, cargarlo en memoria
- Cargar los registros del nuevo (si no era nuevo)
- Poner el valor del IP nuevo

La tabla de procesos del sistema operativo es la colección de los
**PCBs** (*process control blocks*) que es donde el SO guarda toda la info
que tiene que saber de un proceso (por ejemplo el pid)

Hacer todo esto *toma tiempo*. Entonces el SO quiere cambiar de proceso de
forma astuta, no cambiar al pedo, porque gastas mas en el context switch que en
la ejecución de cada proceso.

Es tan importante que llevó a dos consecuencias a nivel de hardware, cambiaron
las arquitecturas para minimizar el tiempo.

Es fundamental determinar la duración del quantum

- Suficientemente largo para amortizar el tiempo que toma el context switch
- Si es muy largo, para que los procesos no acaparen demasiado, y que haya
  alternancia entre los procesos.

Para implementar el quantum, el SO se cuelga de la interrupción del clock.

En la tabla de interrupciones, cuando recibe la int. de clock, es el código que
lleva la cuenta de que proceso ejecuta, hace cuanto, y si debería cambiar si
pasó su quantum.

Es la manera en la que el SO **fuerza que un proceso no se ejecute más de**
**determinado tiempo.**

[imagen cambio de contexto diapo 13]

## Hacer una syscall

Es hacer una llamada al sistema, por ej. `fork()`.

También hay llamadas al SO en actividades mas comunes, imprimir en pantalla
a la larga termina siendo un `write()`.

La diferencia con una función común y corriente es que en las syscalls
interviene el kernel. Por eso, es necesario hacer un *cambio de nivel de*
*privilegio*, que puede requerir forzar una interrupción.

Una syscall es una llamada que invoca al kernel, lo cual toma tiempo.

### POSIX

Por eso aparece una especificación, **POSIX**: Portable Operating System
Interfaze; X: Unix.

IEEE 1003.1/2008

Se estandariza la manera de UNIX, y eso hace que sea el estándar de la syscall,
porque cada SO unix lo hacía como quería, y reducía la portabildiad de los
programas.

## E/S

Es *muy* lenta. Acceder al hardware toma mucho tiempo.

Si un proceso me pide hacer E/S, no me voy a poner a desperdiciar los ticks
que le queda de su quantum en esperar al dispositivo, entonces le termina
el quantum.

No solo no le termina el quantum, sino que también lo saca de la lista de
procesos. El proceso queda bloqueado hasta que termine su E/S

Típicamente hay un registro en la controladora del disco
que indica que el disco terminó con el pedido, y que en otro registro está
el dato que se leyó.

- Busy waiting: Cada tanto se lee el registro hasta que está listo
  No se libera la CPU porque el procesador está fijandose todo el tiempo.
  En general no es la mejor forma.

- Polling: Es como busy waiting pero esperando en el medio. No buscando todo
  el tiempo. El proceso sigue estando listo, nunca se bloquea.

  Polling es multitasking colaborativo, el SO no sacaba a los .

- Interrupciones: Esto permite la multiprogramación.

  El SO recibe la interrupción de que terminó, y lo único que hace es pasar
  el proceso que estaba a su espera de bloqueado a listo.

## Estado de un proceso

!! central !!

Los procesos tienen estado
[img diapo 21]

- Corriendo: Usando la CPU
- Listo: No está corriendo pero está esperando su turno, nada le impide ejecutar
  cuando sea su turno.
- Bloqueado: Uno que pide E/S.
- Terminado: Hizo `exit()`.

### Carga del sistema

Que tan **cargado** está un sistema? Tiene que ver con cuantos procesos tengo
listos. Si están bloqueados, estás esperando al hardware, pero listos están
esperando al procesador.

El comando `uptime` dice la carga del sistema en el último min, 5 min y
ultimos 15 El promedio en la cola de listos.

    $ uptime
    18:43  up 63 days, 4 hrs, 2 users, load averages: 2.94 2.76 2.88

En general uno trata de que ese número esté por abajo del doble de la cantidad
de procesadores. (una right-hand rule) (¿¿¿por qué???)

## Definición de proceso

Se puede ver desde distintos lugares o puntos de vista

- Espacio de memoria
- Entry en la tabla de procesos (PCB)
- Unidad de scheduling

## IPC (comunicación entre procesos)

Además de hacer E/S, InterProcessCommunication (IPC).

Hay varias formas

- Memoria compartida
- Otro recurso compartivo (archivo, BD, etc.)
- Pasaje de mensajes (mecanismo primitivo)
  Un proceso le puede enviar un mensaje a otro proceso. (en esto nos centramos)

### Pasaje de mensajes

- Unix SySV Transport Layer Interface
- **BSD Sockets**. Esta es la línea evolutivamente ganadora.

Los **Sockets** son la forma en la que usualmente se hace IPC.

Es un *extremo* en la comunicación. Como un enchufe en la pared.

Cuando creo un socket lo tengo que conectar a un cable y una vez que está así
lo puedo usar para *leer* o *escribir*.

Por ejemplo

    s = open(...)   // creo el socket de alguna manera, que determina con qué
                    // me termino comunicando.
    var = read(s)
    write(s, var)

Tanto el acceso a los archivos y a todo termina siendo medio lo mismo que un
socket.

Hecho de esta forma, podría ser un socket que se conecta a otro proceso, o
a la parte de TCP/IP de mi computadora, que termina mandando un mensaje a través
de internet.

### Sync o async

!! TODO: Leer diapo 26 !!

La comunicación entre procesos puede ser sincrónica o asincrónica.

- Sincrónica: Como una llamada por teléfono, me quedo escuchando
- Asincrónica: No me quedo esperando la respuesta, no es en el momento. Por ej.
  mail.

## Resumen de la clase

- Que hace un proceso
- Actividades que puede hacer
  - Terminar
  - Multiplicarse
  - Ejecutar en el CPU (aparece quantum y context switch)
  - Invocar al kernel para hacer las cosas de máximo privilegio
  - E/S (es *fundamental*)
  - syscall
- Intro al scheduler
- Intro a IPC

## Bibliografía

[diapo 28]
