# Memoria

## Introducción

Nos volvemos a meter adentro del kernel, y vamos a analizar la memoria

La frustración de hoy: La memoria también se comparte.

Que significa manejar la memoria?

- Entender que cachos de memoria tengo libres y cuales ocupados, y por quien.
- Asignar y liberar la memoria dandosela a un proceso.
- Controlar el swapping: que hago cuando los procesos van y vienen de disco.

!!!! No se dice **alocar**, sino **reservar** memoria.

Hay varios procesos que comparten memoria.

## Swapping

Si pensamos en multiprogramación, cuando un proceso está bloqueado y ponemos
a ejecutar otro, que hacemos con su memoria?

El mecanismo mas sencillo es **swapping**: agarro un proceso que está bloqueado
y toda la memoria que tiene la grabo en el disco

Problemas: Tarda mucho más la conmutación, es **muy** lento.

Pero puede haber mas procesos que memoria disponible en la máquina. No todos los
procesos que están cargados tienen toda su memoria en memoria.

## Administrar memoria

Si tengo 8Gb pero 1600Gb de necesidad, voy dejando en los 8GB "los más usados"
o los "más recientemente usados", pero en algún momento lo voy a tener que bajar
a disco. Si al subirlo devuelta se sube en otra porción de memoria, y el
proceso usaba direcciones físicas, se rompe todo.

Se usa un registro de base, y todas las direcciones se toman como relativas
a esa base.

Pero como aseguuramos que cada proceso acceda solo a su memoria?

- **Reubicación**: Cambios de contexto / swapping
- **Protección**: Memoria privada de los procesos.
- **Manejo de espacio libre**: Evitando la fragmentación

Podría separar a la memoria en fragmentos fijos.

## Fragmentación de memoria

> Había una cantidad de espacio libre para meter un auto más, pero partido
> en pedazos tan chiquitos que no entraba a fines prácticos.
> La calle estaría **fragmentada**

La fragmentación es cuando tenemos suficiente memoria para etender una
solicitud, pero no es continua. Si no se soluciona, el problema se hace
arbitrariamente grande.

- Cada tanto pará todo y compactalo, pero tarda mucho tiempo, entonces es
  imposible en los SOs RT (real time)

La idea es *evitar* la fragmentación

- Segmentos
  - Stack: Crece hacia abajo. Variables locales.
  - Heap: Crece hacia arriba. Memoria dinámica.

- **Bitmap**
    Es un array de 1s y 0s. Pongo un 1 cuando está ocupado, y un 0 cuando está
    libre. Divido la memoria en bloques de igual tamaño.
    Cuando quiero asignar un bloque, le pongo un 1, y cuando lo quiero liberar
    le pongo un 0.

    El problema es que para encontrar si hay un bloque libre, hay que hacer
    una barrida lineal, lo cual tendría costo O(n).

- !!**Lista enlazada**

    Cada nodo representa o un bloque libre o el proceso que tiene la memoria.
    Si es un bloque libre, figura su tamaño y sus límites.

    Liberar siguue siendo O(1).

    Los bloques libres no son de tamaño fijo. Cada uno digo de que tamaño es.
    No resuelve del todo la fragmentación.

    La ventaja que tiene es que como los bloques se agrupan hay mucho menos
    para recorrer.

Ej diapo 10.

### En donde lo ubico

Donde asignar en uuna lista de bloques

- **First fit**: El primer lugar donde entre.
  > Me piden un bloque de 10K, si tengo al principio un bloque de 11K, lo asigno
  > ahí aunque quede ese K libre, a pesar de que al final haya uno de
  > exactamente 10K
- **Best fit**: Me fijo donde entra justo. Recorro toda la lista para verlo.
  - Es mas lento
  - Tampoco es mejor, en la práctica también tiende a fragmentar la memoria
- **Quick fit**: Es una variación de best fit que mantiene además una lista de
  los bloques libres de los tamaños más frecuentemente solicitados.
- **Buddy system**

Fallan todos porque son muy ingenuos. En la práctica son necesarios esquemas
más complejos pero inteligentes. Y los procesos están pidiendo memoria todo el
tiempo, entonces queremos que sea rápido en la asignación de memoria pero
tiene que ser eficiente en el largo plazo.

### Tipos de fragmentación

Queremos resolver o evitar dos tipos de fragmentación

- **Fragmentación externa**: Quiero pedir un bloqueu y no hay ningún bloque de
  memoria contiguio que me permita satisfacer el pedido.
- **Fragmentación interna**: Un proceso me pide un bloque de 10K, se lo doy, pero
  en realidad de esos 10 usa 8. O me pide uno de 2, y como el tamaño mínimo es
  de 4 le doy de 4, entonces está asignado de 4 pero solo quería 2.

> Design of a General Purpose Memory Allocator for the 4.3BSD UNIX Kernel"
> (MK McKusick y J Karels.)

## Memoria virtual (Reubicación)

Repaso de orga2

!! Vamos a combinar *swapping* y el *virtualización de espacio de direcciones*.
A lo cual se lo llama **memoria virtual**.

Requiere ayuda del hardware por la MMU (memory management unit).

{imagen diapo 16}

Sin memoria virtual:

- El espacio de dir está limitado al tamaño de la física
- Obtener una celda
  - Poner la dir en el bus de memoria
  - Obtengo el contenido

Con memoria virtual:

- El espacio de direcciones es el tamaño de la mem física + swap.
- Los programas usan *direcciones virtuales*
- Obtener una celda
  - Pongo la dir en el bus de memoria
  - La MMU traduce esa dirección a física
  - La tabla tiene un bit quue indica si el pedazo está cargado en memoria o
    si está en disco porque fue swapeado.
  - Si no lo está hay que cargarlo.
  - La dirección física se pone en el bus quue llega a la placa de memoria
  - Obtengo el contenido.

Esto me permite direccionar un espacio mayor, y además que no haya problemas
si al cargar la memoria de un proceso queda en una dirección física diferente.

### Paginas

La memoria virtual está dividida en páginas de tamaño fijo y el de memoria
física en bloqueus del mismo tamaño llamados *page frames.*

La MMU traduce páginas a frames. Interpreta las direcciones como

    página + offset
    31    ...     0

Lo que se swapea son siempre páginas.

Cuuando una no está en memoria la MMU la tiene que traer. Para esto emite una
*Page Fault*, que es atrapada por el SO. Como podría ser que tenga la memoria
toda llena, tal vez tengo que desalojar otras. **Cuál** sacar puede afectar
mucho al rendimiento.

### MMU

Cómo sabe cuál es el mapeo entre dir físicas y virtuales? Tiene una estructura
de páginas multinivel. Guarda en memoria la tabla de páginas.

Figura diapo 22.

Que tiene cada PTE?

- Page frame
- Bit de ausencia / presencial
- Bits de protección. Su complejidad depende del SO
- Bit de dirty: indica si la página fue modificada desde la última vez que
  la cargué de disco. **Si no fue modificada, la puedo desalojar sin escribir.**
  De esta forma *un bit* me da mucha performance.

- Bit de referenciada: Indica si fue accedida desde que se trajo de disco.

### Memoria asociativa

Ante cada pedido de acceso del procesador a memoria tiene que traducir. Y los
procesos están *todo el tiempo* accediendo a memoria. Y los accesos a memoria
en terminos del procesador son *lentos*.

Y si la tabla de páginas es multinivel, como también está en memoria, tal vez
hay que hacer muchos accesos y sería muy muy lento.

Esto se soluciona con un cache: **la TLB** (Translation Lookaside Buffer).

- Se implementa con registros muy rápidos
- Tiene la traducción de las páginas a frames mas frecuentemente usadas.

### Reemplazo de páginas

Que páginas dejo en memoria y que páginas reemplazo?

Algoritmos:

- FIFO: ingenuo
- Segunda oportunidad: Hago como FIFO, pero si a la hora de bajarla tenía el
  bit de *referenciada*, el doy una segunda oportunidad y le apago el bit.
  Le doy una segunda vida si fue leída.

Hay muchos más algoritmos, diapo 26.

- LRU: Menos frecuentemente usado. Es el que suele funcionar mejor en la
  práctica. Trato de sacar la que es menos usada.
  Para saber si fue usada recientemente, le agrego al procesador un contador
  de instrucciones. Implementación: p.26

### Páginas especiales

- Paginas de solo lectura
- Páginas no swappeables (seguridad): No tenés control de lo que pasa en el
  disco. El borrado en general es lógico y el borrado físico no es tan dificil
  de revertir.

### Page Fault

Es el momento en el que se chequean los permisos. Si el proceso quiere acceder
a una página que no le corresponde,

- No la tiene mapeada
  - No le corresponde
  - No la tiene todavía en memoria
- Tiene los permisos

Ahí sale la *segmentation violation* que lo hace terminar.

para ver en detalle, diapo 28

## Thrashing

Cuando lo alcanza la memoria y hay mucha competencia entre los procesos por
usarla. Estoy todo el tiempo trayendo páginas de disco.

Prácticamente cada operación de memoria se transformaba en una de disco.

Es una situación indeseable. Es uno de los síntomas.

## Copy on write

Se crean nuevos procesos a través de fork(). Al principio se le mapean las
mismas páginas que el proceso padre, hasta el momento en el que hace la primera
escritira, antes de escribirla la duplico.

## API de administración de memoria

- SO
  - Memoria de kernel
  - Memoria de procesos
- Lenguajes de programación
  - Explícita
    - Bibliotecas: `malloc()`, `free()` en libc

d. 38
