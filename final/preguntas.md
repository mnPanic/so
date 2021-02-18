<!-- markdownlint-disable MD033 -->
<!-- MD033/no-inline-html: Inline HTML -->
# Preguntas de finales por tema

**Disclaimer**: Las respuestas son mias, no necesariamente estan bien.

<!-- Para respuestas colapsadas
  <details>
  <summary>Respuesta</summary>


  </details>
-->

## Procesos y API

- "Para soportar threads alcanza con agregar un atributo de thread id a la PCB." Justificar (Era un poco más largo pero
  era la idea) ([16/05/19])

  <details>
  <summary>Respuesta</summary>

  No, hay que expandir la PCB del proceso con toda la informacion de cada thread que sea distinta, como los registros,
  el program counter, stack, etc, estado de scheduling y un puntero al proceso que contiene el thread.
  </details>

- Describa los distintos mecanismos de IPC. ([16/05/19])

  <details>
  <summary>Respuesta</summary>

  - Recursos compartidos
    - Shared memory: Permite usar primitivas de sync en la memoria compartida
  - Message passing: sync o async 
  </details>

- (creo) por que puede pasar que tenes muchos procesos en waiting, y como podria
  tratar de arreglarlo si no pudiese agregar memoria

- Una página es compartida por dos procesos. Puede suceder que para un proceso sea de solo lectura mientras que el otro
  tenga permitida la escritura? ([19/12/19], [16/05/17])

  <details>
  <summary>Respuesta</summary>

  Si, porque los page table entries que son los que determinan los permisos de acceso estan en cada page table de cada
  proceso.
  </details>

- Que es una system call y para que sirve? ([09/03/18], [09/08/17])

  1. explicar detalladamente las los pasos involucrados diferenciando partes de hardware y software.

  <details>
  <summary>Respuesta</summary>

  Una system call o *syscall* sirve para que un proceso de usuario pueda acceder a servicios del sistema operativo.

  Pasos:
  - Un usuario hace un syscall probablemente mediante otra cosa de mas alto nivel, como una funcion de libc
  - Esa syscall genera una interrupcion (software interrupt, trap), dejando al proceso en waiting
  - Llega la interrupcion al procesador, se genera un mode switch a kernel, va al handler de interrupciones indexa en un
    vector que indica el handler especifico para la interrupcion de syscall
  - El handler hace lo que tenga que hacer y luego devuelve el resultado al usuario
  - El proceso pasa a estar en ready, y en algun momento se volvera a ejecutar
  </details>

- Puede un proceso modificar su PCB? ([09/03/18])

  1. diferencias entre un proceso creado con fork() y un thread
  2. (habia mas items)

  <details>
  <summary>Respuesta</summary>

  Un proceso no deberia poder cambiar su PCB, suena peligroso. Por ejemplo si cambia el instruction pointer o algo, pero
  ahi simplemente daria un segmentation violation o cosas asi.

  Un proceso creado con fork() tiene un solo hilo de ejecucion, es un proceso a parte que no comparte stack ni nada. Un
  thread es un hilo de ejecucion que es parte del mismo proceso, comparten memoria y son mas lightweight.
  </details>

- Se poseen dos procesadores y se pueden ver los cambios de estado de los procesos. ¿Qué tendría que ver para que se
  aumente el rendimiento agregando... ([19/06/18], [31/07/18])

  1. ...un procesador?
  2. ...más memoria?

  <details>
  <summary>Respuesta</summary>

  1. Que el load average este por arriba de la cantidad de procesadores.
     Si se puede ver el cambio de estado de los procesos, que haya muchos procesos en READY o que haya muchos que esten
     pasando de running a ready.
  2. Que haya muchos procesos en waiting (o pasando de running a waiting), por lo que agregando mas memoria principal
     habria que ir menos a disco y habria menos procesos que se queden bloqueados por IO.
  </details>

- Explicar diferencia entre proceso y thread y relación de este último con funciones reentrantes. ([05/03/20], [09/08/17])

  <details>
  <summary>Respuesta</summary>

  Un proceso puede tener multiples hilos de ejecucion, que son los threads. El thread es la unidad minima de scheduling
  y ejecucion del CPU.

  > reentrant code: Code that supports multiple concurrent threads (and can thus be shared). (silber)

  Una funcion reentrante es aquella que puede estar ejecutada concurrentemente por multiples threads.

  Arbol de procesos es el hecho de que cuando un proceso hace un `fork()` tiene un proceso hijo, que puede hacer otro
  `fork()` y asi. Los organiza jerarquicamente.
  </details>

- explicar qué es un árbol de procesos y cuál es su importancia ([05/03/20])

  <details>
  <summary>Respuesta</summary>

  Arbol de procesos es el hecho de que cuando un proceso hace un `fork()` tiene un proceso hijo, que puede hacer otro
  `fork()` y asi. Los organiza jerarquicamente.
  </details>


## Sync

- como podrías sincronizar dos procesos usando IPC? ([19/12/19])

  <details>
  <summary>Respuesta</summary>

  - Con memoria compartida, ahi se pueden agregar cualquier primitiva de sincronizacion.
  - Con pasaje de mensajes

  </details>

- Cuál es la diferencia entre Spin locks y semáforos? Cuándo utilizaría cada uno? ([19/12/19], [16/05/19])

  <details>
  <summary>Respuesta</summary>

  Un spin lock gasta tiempo de CPU hasta que se libere un lock, esta *spinning* (en un ciclo) preguntando si se levanto.
  Hace busy waiting. En cambio, un proceso que espera en un semaforo se bloquea y eventualmente es levantado.

  La eleccion depende del nivel de contencion. Si hay poca contencion, un spinlock va a ser mas rapido porque espera muy
  poco tiempo, a diferencia de un semaforo que tiene el overhead del context switch y de dormirse.

  En cambio si hay mucha contencion y se va a tener que esperar por mucho tiempo, el semaforo aprovecha mejor el CPU.

  </details>

- Considere el modelo de memoria compartida. ([09/03/18])

  1. Dé dos primitivas de sincronización.
  2. Dé implementaciones para cada una de ellas. Explique y justifique todo lo que asuma.
  3. Explique ventajas y desventajas de cada una.

  <details>
  <summary>Respuesta</summary>
  1. Semaforos, TestAndSet

  2.

  ```c
  # semaforos
  # las implementaciones deberian ser atomicas

  type semaphore int
  wait(sem) {
    sem--
    if sem <= 0 {
      dormir()
    }
  }

  signal(sem) {
    sem++
    if sem <= 0 {
      despertar_alguno()
    }
  }

  # TestAndSet
  # la idea es que hace un set y devuelve el valor anterior
  # atomica porque se implementa desde el hardware
  bool test_and_set(bool* dest) {
    bool prev = *dest
    *dest == true
    return prev
  }
  ```

  3. Semaforos tiene la ventaja de que es mas alto nivel y permite hacer exclusion mutua sin busy waiting (depende de
     como se implemente, segun el silber la definicion clasica es con busy waiting, lo que no tiene tanto sentido). Es
     mejor tambien para cuando se usan *counting semaphores*, son mas versatiles que TAS.

    Test_and_set es mas lightweight y mas rapido, tiene menos overhead, ya que no hay cambios de contexto para poner el
    proceso a dormir y no hace falta que frene y resuma. Es mejor para cuando el tiempo por el cual se espera el lock es
    bajo, y cuando no hay tanta contencion.

  </details>
- Elegir una primitiva de sync para los siguientes casos y justificar ([25/04/18], [17/06/18]):

  Habíá que sincronizar procesos para:
  
  A) acceso exclusivo a disco

  B) acceso a una estructura que soporta hasta 3 procesos concurrentes

  C) acceso exclusivo a un contador que se quiere incrementar

  <details>
  <summary>Respuesta</summary>

  a. Necesitamos garantizar exclusion mutua. Podemos usar semaforos o spinning, dependiendo de por la cantidad de tiempo
  que se planea estar bloqueados (corto spinning, largo sem)

  b. Semaforos, porque con un counting semaphore es facil tener esa exclusion mutua. Sino habria que tener un mutex y un
  contador lo cual seria menos eficiente

  c. Basta con un entero atomico, que tenga un incremento atomico mediante hardware, que sera lo mas eficiente.

  </details>

- Justificar si la primitiva de sincronización propuesta para cada caso es correcta, y sino cuál usaría y por qué:

  1. Acceso exclusivo a disco utilizando TAS Lock.
  2. Acceso a una estructura que permite hasta 3 accesos simultáneos con semáforos.
  3. Acceso a un contador que se desea incrementar mediante un semáforo binario.

  <details>
  <summary>Respuesta</summary>

  1. No esta mal, pero dependiendo de por cuanto tiempo se bloquee podria llegar a ser mejor usar un mutex con un
     semaforo binario.
  2. Si, en particular un counting semaphore.
  3. Seria mas eficiente con un entero atomico.
  </details>

- Explicar deadlock con un dibujo y en pocas palabras ([05/03/20])

  <details>
  <summary>Respuesta</summary>

  Deadlock es cuando hay un conjunto de procesos que estan esperando por recursos que el resto del mismo conjunto posee.

  Por ejemplo P1 espera por R1, que tiene adquirido P2, que espera por R2 que tiene P1. No hay progreso porque estan
  *bloqueados* para siempre esperando esos recursos. Deadlock!

  ```text
  P1 --> R1 --> P2
   ^             |
   |             |
   +---- R2 <----+
  ```

  </details>

## ES

- como afectan los dispositivos de memoria de estado solido a los algoritmos de scheduler de E/S ([19/12/19])

  <details>
  <summary>Respuesta</summary>

  Los algoritmos de scheduling de disco suelen ser para evitar que se mueva mucho el cabezal de los HDDs, minimizar el
  seek time y eso. Como las SSDs son electronicas en vez de mecanicas, todo eso no es necesario, y por lo general no
  vale la pena hacer mas que un simple FCFS.
  </details>

- definir Storage Area Network (SAN) ([19/12/19], [05/03/20])

  <details>
  <summary>Respuesta</summary>

  Storage area network: Acceder al storage a traves de la red es lento. Una SAN es una red privada que conecta
  servidores y unidades de almacenamiento

  </details>

- Por qué los algoritmos de scheduling se miden en cilindros. ¿Está bien? ¿Por qué? ([05/03/20])

  <details>
  <summary>Respuesta</summary>

  Supongo que se refiere a tiempo que se tarda en mover de cilindro a cilindro, tambien conocido como seek time. Se
  miden en eso porque es la operacion que mas tarda.

  Por SAN supongo que se refiere a SCAN (algoritmo de ascensor). Este va de un lado al otro del disco (escaneando),
  tomando los pedidos que encuentra en el camino. Luego vuelve en orden reverso.

  </details>

## Scheduling

- ¿Qué es el problema de inversión de prioridades y qué schedulers afecta? ([09/08/17])

  <details>
  <summary>Respuesta</summary>

  Afecta a los schedulers que tienen una politica segun prioridades. El problema sucede cuando un proceso de prioridad
  alta termina esperando a uno de prioridad menor que el para correr.

  Por ejemplo, procesos L < M < H con esas prioridades. Si H esta esperando un recurso que tiene L, y entra M para
  ejecutar, L es preempted y H pasa a esperar a que termine M (de menor prioridad) para que luego ejecute L y pueda
  liberar el recurso.

  Una solucion posible es la de heredar prioridades, si H esta esperando un recurso que tiene L, L pasa a heredar la
  prioridad de H para que no sea preempted por uno de menor.
  </details>

- ¿Qué desafíos enfrentan los algoritmos de scheduling para sistemas con varios procesadores? ([09/08/17])

  <details>
  <summary>Respuesta</summary>

  Afinidad de los procesos a los CPUs, por ejemplo por la cache. Una forma de solucionarlo es con colas por procesador,
  pero ahi podria pasar que uno este muy ocupado y otros muy libres. Para hacer load balancing se puede hacer *push
  migration* (el que esta sobrepasado le pushea a los demas) y *pull migration* (los que estan al pedo le sacan al que
  tiene.)
  </details>  

- Defina starvation y de un ejemplo ([16/05/19])

  <details>
  <summary>Respuesta</summary>

  Starvation se da cuando un proceso no logra progreso. Se puede dar con ciertas politicas de scheduling, por ejemplo
  con una politica *SJF* (Shortest Job First), uno que tenga un trabajo largo podria quedarse esperando indefinidamente.
  </details>

- El enunciado era un output ficticio del comando 'uptime' para un sistema con 2 procesadores y basado en los valores
  de load_average (man uptime) tenías que decir si mejoraba el rendimiento en el corto y en el largo plazo agregando un
  cpu. ([25/04/18])

  <details>
  <summary>Respuesta</summary>

  El load average de `uptime` te dice cuantos procesos hay en ready o ejecutando en CPUs, pero no normalizado a la
  cantidad de CPUs.

  > the system load averages for the past 1, 5, and 15 minutes.
  >
  > System load averages is the average number of processes that are either
  > in a runnable or uninterruptable state.  A process in a runnable  state
  > is  either using the CPU or waiting to use the CPU.  A process in unin‐
  > terruptable state is waiting for some I/O access, eg waiting for  disk.
  > The  averages  are  taken over the three time intervals.  Load averages
  > are not normalized for the number of CPUs in a system, so a load  aver‐
  > age  of 1 means a single CPU system is loaded all the time while on a 4
  > CPU system it means it was idle 75% of the time.

  Habria que ver una estadistica del valor de uptime a lo largo de un dia normal de uso por ejemplo, porque con solo 15
  minutos no podriamos tomar una decision, podria ser un pico de utilizacion del sistema o justo un momento en el que
  estuvo con poco uso.

  Para un sistema de dos procesadores,

  - Si tuvo en general abajo de 2, no tendria mucho sentido agregar otro procesador porque no se usaria, no hay
    suficiente carga.

  - Si hubo mas, es porque estuvieron al 100% de utilizacion y podrian delegar procesamiento a otro mas.
  </details>

  a) Decir si agregando memoria podría mejorar el rendimiento para la situación del ejercico anterior.
  b) describir atributos de página
  c) pregunta si 2 procesos pueden compartir páginas de memoria y para qué serviría

  <details>
  <summary>Respuesta</summary>
  a. Solo si estan mucho en waiting, lo cual no lo ves con uptime
  b. Supongo que atributos de la tabla de pagina, son

  - Bit de dirty: Para saber si se modifico, util para algoritmos de reemplazo de paginas.
  - Bit de present: Para tirar PF cuando no esta en memoria, necesario para demand paging.
  - Bits de proteccion
  - Bit de referenced: Accedida desde que fue cargada a disco. Util para algoritmos.
  - Puede tener mas

  c. Si,

  - Para hacer IPC con memoria compartida
  - Para evitar cargar mas de una vez paginas que son compartidas por muchos procesos, por ejemplo de libc.

  </details>

## Memoria

- Que es thrashing y como se puede solucionar sin agregar hardware? ([09/08/17])

  <details>
  <summary>Respuesta</summary>

  Thrashing es cuando el CPU pasa mas tiempo swappeando paginas de memoria principal a disco que corriendo procesos.
  Puede darse porque hay mas memoria asignada a procesos que disponible en el sistema, y los procesos la usan toda a la
  vez.

  Soluciones sin agregar hardware:

  - PF rate: Tener una cota superior de page faults posibles, y matar a un proceso cuando nos pasemos para evitar
    thrashing.
  - Decrementar nivel de multiprogramacion
  </details>

- De dos estructuras para el manejo del espacio de memoria libre. Hable sobre sus ventajas, desventajas y diferencias
  ([16/05/19])

  <details>
  <summary>Respuesta</summary>

  - Bitmap: Dividir la memoria en bloques de tamaño fijo, y usar un bitmap donde 1 es ocupado y 0 es libre. asignar y
    liberar es O(1) pero buscar un bloque libre es O(n)
  - Lista enlazada: Cada nodo representa un bloque libre o uno tomado por un proceso, liberar y asignar es O(1)

  </details>

- Recomendaría hacer backups en un sistema que ya cuenta con RAID? Justifique incluyendo descripción de ambos.
  ([16/05/19])

  <details>
  <summary>Respuesta</summary>

  Si, porque RAID solo te salva de fallos del medio fisico agregando redundancia (o da velociades de lectura y escritura
  mas rapidas) pero no de fallos de software. No te salva de hacer `rm` sin querer pero un backup si.
  </details>

- ¿Qué es RAID? ¿Qué resuelve? ¿Qué variaciones hay? (le conté de R0, R1 y R5 nomás) ¿Hay algo que no resuelva? ([12/12/19])

  <details>
  <summary>Respuesta</summary>

  RAID, Redundant Array of Independent Disks, es un protocolo por el cual se usan muchos discos fisicos para representar
  otra cantidad de discos logicos. Agrega *redundancia* a un sistema, resiliencia ante fallos fisicos. Lo que *no*
  resuelve es fallos de software o del usuario, si se hace un mal uso del disco RAID no te salva, por eso siempre hay
  que hacer backup.

  Las variaciones se organizan en niveles
  - R0: non-redundant striping: se splitean los bytes en multiples discos para que asi las lecturas sean mas rapidas,
    pero no es mas resiliente.
  - R1: mirrored disks: duplica todo, las escrituras se hacen en ambos y las lecturas se pueden hacer concurrentes y te
    quedas con el que responda primero
  - R4: block-interleaved parity: se agrega informacion de paridad en un disco extra, pero tiene el overhead de computar
    el parity
  - R5: Distribuye la paridad entre los discos
  - 0+1: combina 1 y 0, hace striping y duplica. Es como duplicar el 1.
  - 1+0: Primero duplica (de a pares) y despues hace striping sobre esos pares.
  </details>

- ¿Cómo funciona protección en paginado? ([12/12/19])

  <details>
  <summary>Respuesta</summary>

  Supongo que se refiere a los bits de proteccion que estan en la page table.
  </details>

- Hablame de page fault ([12/12/19])
  - ¿Que pasa cuando salta un page fault cuando una pag no está en memoria? Describir el proceso.

  <details>
  <summary>Respuesta</summary>

  Con demand paging no estan todas las paginas de un proceso en memoria cuando este se carga. Por lo tanto, cuando
  accede a una pagina que no esta se genera un Page Fault (un trap, software interrupt) que es atrapada por el SO para
  traer el page frame a memoria principal.

  Los pasos son:

  - El proceso referencia una pagina que no esta en memoria
  - Se lee en una tabla interna (page table) usualmente dentro de la PCB del proceso para determinar si era una
    referencia valida o un acceso invalido.
  - Si era invalido se mata al proceso, pero si no estaba en memoria generamos un page fault (trap)
  - A la hora de handlear el page fault, se encuentra un frame libre (algoritmos de reemplazo)
  - Se agenda una operacion de secondary storage para traerla a memoria
  - Cuando termina, se modifica la PT para indicar que ahora esta en memoria
  - Se baja la otra de ser necesario (si esta dirty, sino no)
  - Se resume la ejecucion del proceso en la instruccion que genero un PF, pero ahora si tiene la pagina.

  </details>

- Defina thrashing y fragmentación, y enuncie para cada caso una solución, con sus ventajas y desventajas ([09/03/18], [03/08/17])

  <details>
  <summary>Respuesta</summary>

  - Thrashing: Con paginacion via demand paging, no se traen todas las paginas de un proceso de una, sino que se traen a
    medida que van siendo necesarias. En un sistema multiprogramado (en el que corren mas de un proceso concurrentemente
    en la CPU), si hay mas espacio asignado a los procesos que el disponible en memoria principal, y se esta usando
    tambien mas que el disponible, van a estar swappeandose paginas de memoria a disco todo el tiempo.

    Soluciones posibles:
    - PF frequency: como trashing hace que incremente el ratio de page faults, a partir de cierto punto si se pasa
      matamos a algun proceso.
    - Agregar suficiente memoria para mas o menos tener suficiente para bancarse el *working set* (paginas que estan
      usando los procesos), ajustando el *degree of multiprogramming* (#procesos ejecutando concurrentemente en la CPU)
  - Fragmentacion: Hay dos tipos
    - Interna: Se desperdicia espacio dentro de la memoria asignada. Se suele asignar bloques de tamaño fijo en vez de
      la cantidad exacta que pida un usuario, entonces por ej. alguien pide 10 bytes pero se le asigna una pagina
      de 4KB, se esta desperdiciando mucho espacio.

      Soluciones posibles:
      - hacer mas chico el tamaño de las paginas para tener mas granularidad.
      - Usar segmentos de tamaños variables para ajustarse mejor a los pedidos, pero podria llevar a fragmentacion
        externa.

    - Externa: Segun como se asignen la memoria a los procesos, en asignacion contigua por ejemplo, podria pasar que
      a medida que se vayan liberando y asignando queden cachitos sin usar entre espacios usados que no puedan
      utilizarse. Se vuelve un problema cuando para un request, hay suficiente memoria libre para satisfacerla pero como
      no es contigua, no se puede satisfacer el request.

      Solucion posible:
      - paginacion, ya que siempre se asignan paginas y se pueden tomar intercaladas, no hace falta que sean contiguas.
        Es mas dificil de implementar.
      - compactar, pero es muy lento
  </details>
- Describir los atributos de las páginas y para qué se usan. ¿Es posible que hayan páginas compartidas? ¿En qué caso?
  ([19/06/18], [31/07/18])

  <details>
  <summary>Respuesta</summary>

  Atributos:

  - Bit dirty: Para no tener que copiarla a disco si solo fue leida.
  - Bits de proteccion
  - Bit referenced: Si fue referenciada desde que se trajo a memoria, usada para algoritmos de reemplazo.
  - Bit present: Para demand paging, si esta cargada en memoria la pagina o si hay que traerla.

  Puede haber paginas compartidas,

  - IPC con memoria compartida
  - Compartir liberias para traerlas una sola vez, como lib.
  </details>

## FS

- Qué es el superbloque en ext2 y que pasa si se pierde su información? ([09/08/17])

  <details>
  <summary>Respuesta</summary>

  El superbloque tiene la descripcion de todo el file system en general, y no pasa nada si se pierde porque se copia de
  forma redundante en todos los grupos de bloques.

  </details>

- Cuál es la diferencia a nivel de operaciones, de cuando se hace ls y ls-l ([09/08/17])

  <details>
  <summary>Respuesta</summary>

  Ls solamente lista los directory entries con sus nombres, mientras que ls -l tiene que entrar al inodo (una operacion
  extra por cada archivo) para leer su metadata.

  </details>

- Se cuenta con un FS basado en inodos que cuenta con un bitmap para mostrar el espacio libre. Si este bitmap se pierde,
  existe alguna forma de reconstruirlo? ([16/05/19])

  <details>
  <summary>Respuesta</summary>

  Si es un bitmap que dice que bloques estan libres y cuales ocupados, una forma poco eficiente de reconstruirlo podria
  ser caminar todo el file system desde el directorio root, siguiendo los inodos para ver que bloques estan ocupados.

  Al terminar, los que no se encontraron ocupados estaban libres y listo.
  </details>

- En ext2 sin journaling, ¿qué problema hay si se corta la luz de repente? ¿Cómo lo resuelvo? ([12/12/19])

  <details>
  <summary>Respuesta</summary>

  Si la luz se corta de repente, podria pasar que algunas operaciones queden a medio hacer, ya sea de metadata o de
  escrituras de los datos a disco.

  Para resolverlo,

  - Usar journaling
  - Hacer algun tipo de consistency checking,
    - Viendo toda la metadata del FS (muy lento, offline)
    - Usando un consistency checker como `fsck` en Unix.
  </details>

- proponer un file system para un dispositivo que se escribe una única vez (y sabes todos los datos que queres escribir)
  y lecturas múltiples (estilo DVD). Detallar como se puede minimizar el espacio utilizado en el dispositivo.
  ([19/12/19])

  <details>
  <summary>Respuesta</summary>

  Como FAT pero asignando todos los bloques contiguos, y que la tabla simplemente diga para cada archivo en donde
  comienza y cuantos bloques seguidos tiene.

  </details>

- explicar journaling en ext3. ¿Qué problema resuelve? ([05/03/20])

  <details>
  <summary>Respuesta</summary>

  El journaling en file systems es usado como forma de recovery en caso de fallas. Los cambios en vez de ser aplicados
  de forma sincronica se escriben en un *journal* (se commitean), que es un buffer circular. Cada vez que se va
  llenando, se realiza la accion que tocaba.

  La ventaja que trae es que

  - Las acciones se completan mas rapido porque se escriben en un buffer y se hacen async.
  - En caso de que se caiga el sistema, al levantar se realizan todas las operaciones que quedaron pendientes en el
    buffer, de esa forma recuperandose si habia una por la mitad.

  De lo que no salva es que se caiga el sistema mientras se escribia al buffer (antes de que termine el commit).
  </details>

- a. Que FS usar para un sistema embebido entre FAT e inodos

  b. Que consuma la menor energía posible ([25/04/18])

  <details>
  <summary>Respuesta</summary>

  supongo que para minimizar el consumo queres minimizar los accesos a disco o algo asi? capaz como FAT hace que tarden
  mas los seek y que se muevan muchos los cabezales y eso es peor? pero si vas a velocidad deberia ser mas rapido inodos
  pero tenes el tema de espacio desperdiciado con los inodos, y la complejidad de implementar ext2 vs fat en un embebido
  que probablemente codeas vos el SO
  </details>

- Diferencia entre soft y hard link, que se puede hacer con uno y no con el otro

  <details>
  <summary>Respuesta</summary>

  En un sistema con inodos, los inodos no tienen el nombre del archivo, sino que este esta en el directory entry que
  apunta a el. De esta forma se pueden hacer *links*. Un hard link no es mas que dos archivos en un directorio que
  apuntan al mismo inodo. Un renombre (alias) del mismo. En cambio, un *soft link* es un inodo nuevo, que dentro de su
  cuerpo tiene una referencia a un archivo. Esto permite

  - Tener un soft link a un archivo que no existe, o que siga existiendo despues de que se borro. Con los hard links,
    un archivo no desaparece hasta que se borran todos los links.
  
  - Los soft links permiten referencias cross file system, ya que la referencia se vuelve a resolver. Esto no se puede
    hacer con los hard links, porque son referencias a numeros de inodo.

  </details>

- Comparar FAT vs INODOS para backup total de disco. Lo mismo para incremental ([09/03/18])

  <details>
  <summary>Respuesta</summary>

  Para hacer un backup *total*, hay que copiar *todos* los archivos a un storage secundario / terciario. Para hacer uno
  incremental, hay que copiar solamente los que hayan sido modificados desde la ultima incremental.

  Para total es mejor FAT porque se puede ver directamente desde la tabla cuales son los bloques usados que hay que
  corresponden a archivos, mientras que con inodos habria que ir inodo por inodo, seguir todas las referencias para ver
  que bloques copiar. (fuente: [doc])

  Para el incremental conviene FAT, ya que la metadata (fecha de ultima modificacion (fuente:
  [osdev](https://wiki.osdev.org/FAT#Directories))) de los archivos esta en los directorios, no es necesario acceder al
  inodo para ver cuando fue modificado. Se ahorra un acceso a disco por archivo.

  </details>

- Se desea implementar una funcionalidad de snapshot. ¿Qué sistema de archivos tomaría como base (FAT o inodos) y cómo
  sería su implementación? ([19/06/18], [31/07/18])

  <details>
  <summary>Respuesta</summary>

  Es como un backup total, conviene FAT porque tenes los bloques de memoria que se estan usando y recorriendola ya podes
  saber cuales son y copiarlos. En cambio con inodos, habria que recorrerlos todos.
  </details>

## Seguridad

- Explicar DAC y comparar DAC en unix y windows ([09/08/17])

  <details>
  <summary>Respuesta</summary>

  DAC: Discretionary access control, los dueños de los recursos definen los permisos de ellos.

  En Linux es con granularidad owner, grupo y resto. Mientras que en Windows puede ser por owner, un usuario arbitrario,
  cualquier cantidad de grupos, etc. Mucho mas flexible.
  </details>

- ¿Que es buffer overflow? Contar un ejemplo. ([12/12/19])

  <details>
  <summary>Respuesta</summary>

  Un *buffer overflow* es cuando se usa una funcion para escribir en un buffer que no tiene en cuenta el tamaño del
  mismo en relacion al tamaño del input, asi escribiendo por fuera de el (overflow) en memoria.

  El ejemplo clasico es el de strcpy, que copia del input al buffer hasta que encuentra un null terminator (`\0`), y se
  puede escribir tanto como uno quiera.

  ```c
  void func(char* buf) {
    char[] dest = char[16];
    // Si buf tiene mas de 16 caracteres, se pisa la memoria
    strcpy(buf, dest);
  }
  ```

  </details>

- describir setUID y buffer overflow y un ataque que incluya ambos ([19/12/19], [09/08/17])

  <details>
  <summary>Respuesta</summary>

  `setuid` es un atributo que tienen los archivos en UNIX que hace que todo proceso que lo ejecute asuma el rol del
  creador. Esto es util para cuando temporalmente necesitas privilegios mas altos para realizar alguna accion.

  Los binarios setuid deberian tener funcionalidad muy restringida para no hacer cualquier cosa con permisos elevados,
  pero suelen ser usados mediante buffer overflow para escalar privilegios. Si se puede hacer un overflow e insertar
  shellcode, se escalo privilegios para ejecutar cualquier cosa.

  </details>

- Como funciona stack canary para protegerse contra buffer overflow, dar desventajas ([09/03/18], [25/04/18], [19/06/18])

  > En canary le interesaba no solo la idea general sino especificamente que sucedia, es decir hubiera servido hablar de
  > que te sirve principalmente cuando te quieren escribir el ret de una funcion, y que se valida cada vez que estas
  > volviendo de la funcion. Hablando que la parte del canary es un agregado del compilador

  <details>
  <summary>Respuesta</summary>

  Un buffer overflow sucede cuando una funcion que escribe en un buffer no le importan los bounds del mismo en relacion
  a la longitud de lo que escribe. De esa forma se puede escribir por fuera, pisando otros contenidos del stack, en
  particular la direccion de retorno, saltando a cualquier lado que ellos quieran. Esto se podria usar para saltar al
  buffer que contiene un shellcode para hacer un exploit, o simplemente para un denial of service.

  Un *stack canary* entonces es un valor que el compilador automaticamente pone antes de la direccion de retorno, y
  chequea al final de la ejecucion, para que si se sobreescribe la ret addr tambien se tenga que pisar el canary, lo
  cual seria detectado.

  Si el hacker logra obtener el valor del canary (por ejemplo leyendo el stack), podria evitar la proteccion.
  </details>

- Se posee el siguiente cliente escrito en C que utiliza encripción asimétrica para enviar la contraseña del usuario
  ingresado a un servidor. Se ingresa el nombre de usuario por parámetro, el cual se copia a un buffer de tamaño fijo, y
  luego se obtiene la contraseña escrito en texto plano de la DB. Esta misma es encriptada y enviada. ([31/07/18])

  ```c
  int main(int args, char* argv[]){
      char[16] usuario;
      char path_clave_priv[] = "/etc/clave.priv";
      char ip_servidor = "192.168.1.1";
      char* contrasena;

      strcpy(argv[1], usuario);
      contrasena = obtener_contrasena_db(usuario);
      token = encriptar(path_clave_priv, usuario, contrasena);
      enviar(token, ip_servidor);
  }
  ```

  Dar dos maneras distintas para que un usuario con acceso al cliente pueda hacerse de la contraseña.

  <details>
  <summary>Respuesta</summary>

  Dos maneras:

  1. Buffer overflow, cambiar el path de la clave privada a uno conocido y la ip a localhost o algo asi para poder
     obtenerla. Esto esta bueno porque no hay ninguna proteccion contra buffer overflow que te salve.
     > Nota: Si con asimetrica al encriptar con la privada lo puede desencriptar con la publica (es decir, cualuquiera
     > puede desencriptar) entonces no es necesario pisar la privada, basta con la IP.

  2. Pisar el codigo para que salte a un shellcode que llame `obtener_contrasena_db` y haga un print o algo asi.
    
  </details>

- explicar qué es una función de hash segura y ejemplificar dos usos en un sistema operativo. ([05/03/20])
  - comparar DAC y MAC

  <details>
  <summary>Respuesta</summary>

  Una funcion de hash segura (tambien de una via) es una funcion de hash que no puede volverse atras, es decir que dado
  h(m) no hay forma (practica) de encontrar m.

  Usos
  
  - Guardar claves de usuarios asi no estan en texto plano, mejor con salts para evitar ataques de enumeracion.
  - Idealmente se podria usar para ver que no este alterado un mensaje, pero si se envia sola alguien podria
    interceptarla, cambiar el contenido y recalcular el hash.

    Para evitarlo, se puede usar una funcion de hash y luego encriptar su contenido. Esto es lo que hace HMAC.

  Comparacion

  - DAC: Discressionary Access Control. Es el que se usa por ejemplo para los archivos de UNIX. Un acceso se controla en
    base a las identidades de los usuarios o grupos. Tiene el problema de que por diseño son *discrecionales*, dejan que
    el owner del recurso les ponga cualquier permiso, y el superuser simpre va a tenerlo (lo cual se presta a
    vulnerabilidades)

  - MAC: Mandatory Access Control. El acceso se rige segun *policies* que son impuestas por el sistema a todos los
    usuarios, y que los usuarios no pueden modificar.
  </details>

## Distribuidos

- Explicar el algoritmo de Lamport de ordenación de eventos en sistemas distribuidos ([14/12/16])

  <details>
  <summary>Respuesta</summary>

  Lamport propone su algoritmo de ordenamiento de eventos argumentando que no es necesario saber *cuando* pasaron, lo
  que involucraria usar algun algoritmo de sync de relojes que no suelen ser *tan* precisos.

  Orden parcial (regla 4) no reflexivo (A -> A no tiene sentido)

  1. Si A sucede antes que B, A -> B
  2. E -> R con E envio y R recepcion del mismo mensaje
  3. Transitividad
  4. Si no vale ninguna de las dos son concurentes. (no es un orden total)

  Implementacion:
  1. Cada proceso tiene un reloj monotonicamente creciente.
  2. Envio mensajes con un timestamp de mi reloj
  3. Cuando me llega un mensaje E, veo mi tiempo. Si mi tiempo es anterior al del envio (t) lo ajusto (t+1) por la regla
     2
  
  Con eso alcanza para construir un orden parcial, y para uno total se pueden romper los empates de alguna forma
  arbitraria.
  </details>

- Un algoritmo de commit distribuido funciona de la siguiente manera: opera sobre una red donde los paquetes pueden
  perderse o demorarse, y cuando un nodo quiere escribir sobre cierto archivo que está replicado en todos los nodos,
  envía un pedido de escritura. Si recibe confirmación de todos los nodos, escribe y le notifica a los demás que pueden
  hacer lo propio. Alguien nota que este algoritmo puede fallar si los nodos se caen entre la escritura y la
  notificación y propone para solucionarlo el envío de mensajes de confirmación de parte de los nodos. ¿Este algoritmo
  resuelve el problema? Justifique. ([09/08/17])

  <details>
  <summary>Respuesta</summary>

  No, porque pasa a ser un problema de acuerdo bizantino, no sabes si te confirmo.
  </details>

- Verdadero o Falso. Justifique. ([16/05/19])

  1. El algoritmo de orden parcial de Lamport requiere que los procesadores tengan acceso a un clock real y preciso.
  2. En términos de una red, grid y cluster son conceptos equivalentes.
  3. En una red lo suficientemente rápida, Two Phase Commit es infalible.

  <details>
  <summary>Respuesta</summary>

  1. Falso, justamente eso es lo que evita, tener que sincronizar relojes para cosas que requieren una precision muy
     fina. Solamente requiere que tengan un reloj monotonicamente creciente.
  2. Falso, un grid es un conjunto de clusters
  3. Falso, lo que hace que falle 2PC es que se caigan los procesos, no que esté lenta la red.
  </details>

- ¿Qué es 2PC? ¿Qué problema resuelve? ¿Qué fallas tiene? ¿Hay algún protocolo que resuelva alguna de esas fallas?
  ([12/12/19])

  <details>
  <summary>Respuesta</summary>

  2PC o two phase commit es un algoritmo de sistemas distribuidos para hacer un commit distribuido, que un conjunto de
  procesos acuerden entre *commit* o *abort* de una transaccion. (si alguno aborta todos abortan)

  El algoritmo consiste en
  - Round 1: Todos le dicen a un proceso distinguido su decision, y este se fija si alguno decidio abortar. Si ninguno
    decidio abortar, decide commit, pero sino aborta.
  - Round 2: Hace broadcast de su decision a los demas.

  El problema que tiene es que si falla el proceso distinguido nadie decide nada. Tiene *weak termination*: solo deciden
  todos si ninguno falla. Se puede solucionar con un *protocolo de terminacion*, en el que los procesos que quedan se
  arreglan para decidir algo, pero no fuciona en todos los casos.
  Por ejemplo, si un proceso decidio 0 (abort), le puede informar a los demas y que todos decidan abortar, pero si el
  proceso distinguido habia decidido antes de fallar, se viola la condicion de que todos deberian decidir lo mismo.
  </details>

- Describir los algoritmos de 2-phase commit y 3-phase commit ([19/12/19])

  <details>
  <summary>Respuesta</summary>

  Resuelven el problema de commit distribuido, en el que un conjunto de procesos debe decidir si hacer commit (1) o
  abortar (0) una transaccion(aplicado en bases de datos distribuidas).

  Condiciones de correctitud:

  - **Acuerdo**: Todos deciden los mismos valores (no hay dos que decidan diferente)
  - **Validez**:
    - Si alguno decide abortar, todos deben decidirlo
    - Si todos comienzan queriendo commitear y no hay fallos entonces se deberia commitear.
  - **Termination**
    - weak: Si no hay fallas todos los procesos eventualmente deciden
    - strong (non-blocking): Todos los procesos que no fallan eventualmente deciden.

  Algoritmos:

  - 2PC:
    - En la primera ronda todos le envian a un proceso lo que decidieron. Este se fija si alguno decidio abortar, y
      sino decide commit.
    - En la segunda ronda hace broadcast de su decision y todos deciden lo mismo que el.
  - 
  </details>

- explicar diferencias entre sistema paralelo y distribuidos. ([05/03/20])
  - explicar los problemas del mecanismo de token passing y sus resoluciones

  <details>
  <summary>Respuesta</summary>

  Un sistema paralelo comparte memoria y uno distribuido no.

  Token passing sirve para exclusion mutua distribuida. Se organizan los nodos en un anillo logico (tipo token ring), y
  todos pasan un token. Cuando quiero entrar a la seccion critica, espero a que me llegue y lo paso cuando salgo.

  Problemas:

  - Hay muchos mensajes circulando cuando no es necesario.

    Solucion: Usar una sol alternativa

    Tengo que conocer a todos. Cuando quiero entrar,

    - Mando a todos un mensaje `solicitud(Pi, ts)` con un timestamp ts (si soy el proceso i)
    - Cada proceso puede responder inmediatamente o encolar la respuesta
    - Entro cuando recibi todos los OK, y al salir respondo todos los pedidos encolados
    - Respondo inmediatamente si
      - No quiero entrar
      - Quiero entrar, todavia no lo hice, y el ts del pedido recibido es menor que el mio (el otro tiene prioridad.)

    No circulan mensajes si no es necesario.

  </details>

<!-- chapa -->
[09/03/18]: https://www.cubawiki.com.ar/index.php/Final_9_de_Marzo_2018_(Sistemas_Operativos)
[25/04/18]: https://www.cubawiki.com.ar/index.php/Final_25_de_abril_2018
[19/06/18]: https://www.cubawiki.com.ar/index.php/Final_19_de_Junio_2018
[31/07/18]: https://www.cubawiki.com.ar/index.php/Final_31_de_Julio_2018
[03/08/17]: https://www.cubawiki.com.ar/images/2/25/FinalSSOO-03-08-17.jpeg

<!-- rodo baader -->
[05/03/20]: https://www.cubawiki.com.ar/index.php/Final_Marzo_2019_(Sistemas_Operativos)
[19/12/19]: https://www.cubawiki.com.ar/index.php/Final_Diciembre_2019_(Sistemas_Operativos)
[12/12/19]: https://www.cubawiki.com.ar/index.php/Final_12_Diciembre_2019_(Sistemas_Operativos)
[16/05/19]: https://www.cubawiki.com.ar/index.php/Final_Mayo_2018_(Sistemas_Operativos)
[09/08/17]: https://www.cubawiki.com.ar/index.php/Final_Agosto_2017_(Sistemas_Operativos)
[14/12/16]: https://www.cubawiki.com.ar/index.php/Final_Diciembre_2016_(Sistemas_Operativos)

<!-- preguntas -->
[doc]: https://docs.google.com/document/d/1QOFw99da5EBd8kaa5tli7xId3tFPLQNIvbWytqu8m7k/edit#