# Little book of semaphores

Notas de [*The little book of semaphores*][book]

[book]: http://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf

## Table of contents

- [Little book of semaphores](#little-book-of-semaphores)
  - [Table of contents](#table-of-contents)
  - [Intro](#intro)
    - [Orden](#orden)
    - [No determinismo](#no-determinismo)
    - [Variables compartidas](#variables-compartidas)
      - [Tipos de interacciones](#tipos-de-interacciones)
  - [Semaphores](#semaphores)
    - [Definición](#definición)
    - [Syntax](#syntax)
  - [Patrones de sincronización](#patrones-de-sincronización)
    - [Signaling](#signaling)
    - [Rendezvous](#rendezvous)
      - [Deadlock](#deadlock)
    - [Mutex](#mutex)
      - [Simetria](#simetria)
      - [Sección crítica](#sección-crítica)
      - [Lock & Unlock](#lock--unlock)
    - [Multiplex](#multiplex)
    - [Barrier](#barrier)
    - [Reusable barrier](#reusable-barrier)
    - [Queue](#queue)
      - [Exclusive queue](#exclusive-queue)
  - [Classical synchronization problems](#classical-synchronization-problems)
    - [Producer-consumer](#producer-consumer)
      - [Implementacion](#implementacion)
      - [Deadlock 4](#deadlock-4)
      - [Buffer finito](#buffer-finito)
    - [Readers-writers problem](#readers-writers-problem)
      - [Lightswitch](#lightswitch)
      - [Starvation](#starvation)

## Intro

### Orden

Decimos que dos eventos son **secuenciales** (secuential) si ocurren uno después
del otro, y **concurrentes** (concurrent) si no sabemos el orden en el cual ocurrieron.

### No determinismo

Los programas concurrentes suelen ser no deterministas, es decir, no sabemos
solo con verlo que sucederá. Por ejemplo

```code
  Thread A              Thread B
1 print yes           1 print no
```

El orden de ejecución podría ser

a1 < b1

o

b1 < a1

### Variables compartidas

Cuando las variables son **locales** a los threads no suele haber problema, ya
que el resto no puede acceder. Pero también puede haber algunas que sean
**shared** entre ellos.

#### Tipos de interacciones

Los tipos de interacciones entre threads se pueden partir en

- Concurrent reads: Dos o mas threads hacen un read, lo cual no suele traer
  problemas de sincronización.
- Concurrent writes: dos o mas threads hacen write
- Concurrent updates: dos o mas threads hacen un read, seguido por un write de
  ese valor modificado.

## Semaphores

### Definición

Es como un entero con tres diferencias

1. Se puede inicializar el valor a cualquier entero, pero luego solo se puede
   incrementar o decrementar. No se puede leer.
   > En el marco de la materia se recomienda inicializarlo a un entero positivo.

2. Cuando un thread **decrementa** el semáforo, si el resultado es negativo, el
   thread bloquea y no puede continuar hasta que otro lo incrementa.

3. Cuando un thread **incrementa** el semáforo, si hay otros threads esperando,
   despierta **a uno** de los que estaba esperando.

Que significa el valor?

- positivo: Es la cantidad de threads que pueden decrementar sin bloquear
- negativo: Representa la cantidad de threads que estan bloqueados y esperando.
- cero: No hay threads esperando, pero si uno intenta de decrementar, bloqueará.

### Syntax

```python
fred = Semaphore(1) # Construye un nuevo semaforo con el valor inicial
fred.signal()       # Incrementa el semaforo
fred.wait()         # Decrementa el semaforo
```

## Patrones de sincronización

### Signaling

A threads sends a **signal** to another to indicate that something has happened.

```python
sem = Semaphore(0)

  Thread A          Thread B
1 a1                sem.wait()
2 sem.signal()      b1
```

De esta forma se garantiza el orden `a1 < b1`.

### Rendezvous

Dos threads tienen un *encuentro* (rendezvous) en un punto.

```text
  Thread A      Thread B
1 a1            b1
2 a2            b2
```

Se busca que `a1 < b2` y `b1 < a2` pero no se impone nada sobre a1 y b1.

"Two threads rendezvous at a point of execution, and neither is allowed to
proceed until both have arrived.

```text
aArrived = Semaphore(0)
bArrived = Semaphore(0)

Thread A          Thread B
a1                b1
aArrived.signal() bArrived.signal()
bArrived.wait()   aArrived.wait()
a2                b2
```

#### Deadlock

```text
Thread A          Thread B
a1                b1
bArrived.wait()   aArrived.wait()
aArrived.signal() bArrived.signal()
a2                b2
```

A llega y bloquea en su wait. B llega y también bloquea en su wait, pues A no
llegó a hacer el signal de aArrived. Ninguno puede proceder y nunca lo harán.

Esto se conoce como un **deadlock**.

### Mutex

Implementa la *exclusión mutua* entre threads.

```text
Thread A            Thread B
count = count + 1   count = count + 1
```

Agregarle semaforos al ejemplo para forzar exclusión mutua de la
variable compartida `count`

```python
mutex = Semaphore(1) # en 1 para que el primer wait no bloquee

Thread A            Thread B
mutex.wait()        mutex.wait()
count = count + 1   count = count + 1
mutex.signal()      mutex.signal()
```

#### Simetria

Como los dos threads están corriendo el mismo código, se dice que la solución es
**simetrica** (*symmetric*). En cambio, si tuvieran que correr código diferente,
se diria **asimetrica** (*asymmetric*).

La soluciones simetricas suelen ser más faciles de generalizar.

#### Sección crítica

Se suele llamar al código que debe ser protegido **sección crítica**
(*critical section*).

#### Lock & Unlock

Una metáfora alternativa es pensar a la sección crítica como un cuarto, en el
cual puede haber solamente un thread a la vez. Los mutexes se llaman *locks*, y
un thread *lockea* (locks) el cuarto después de entrar, y lo *desbloquea*
(unlocks) al salir.

### Multiplex

Es una generalización de mutex, pero puede haber multiples threads en la sección
crítica, con una cota superior. No puede haber más de n threads en la sección
crítica a la vez.

```python
multiplex = Semaphore(n) # los primeros n no bloquean

Thread K
mutex.wait()            # si hay menos de n, entra. Sino, espera a que lo haya
    critical section
mutex.signal()          # hay uno menos

# - n representa la cantidad de threads que pueden estar a la vez en CRIT
# - El valor del semaforo representa los que todavia pueden entrar, ya que
#   cuando sea 0, el proximo que lo intente bloqueara hasta que uno salga.
```

A este patron se lo llama **multiplex**

### Barrier

Generalizacion de Rendezvous pero para mas de dos threads.

```python
# Barrier
rendezvous
critical point
```

```python
n = cant threads
count = 0
mutex = Semaphore(1)
barrier = Semaphore(0)

Thread
rendezvous # Punto de encuentro

# Incremento la cantidad que llegaron
mutex.wait()
    count += 1
mutex.signal()

# Si ya llegaron todos, no hay que esperar mas
if count == n: barrier.signal()

# turnstile
barrier.wait()      # Espero a que llegue el ultimo
barrier.signal()    # Despierto al siguiente

critical point
```

El patron de hacer wait, inmediatamente seguido por un signal se suele conocer
como **turnstile**, porque deja que un thread pase a la vez, y puede estar
lockeado para parar a todos. Un turnstile es como el del subte.

En el estado inicial, (cero) el turnstile esta bloqueado. El n-esimo thread la
desbloquea y luego el resto pasa.

### Reusable barrier

Un set de threads realizan una serie de pasos en un loop, y se sincronizan en
una barrera luego de cada uno. Necesitamos una *barrera reutilizable*, que se
lockee por si misma luego de que hayan pasado todos los threads.

Para esto, se pueden usar dos *turnstiles*. Inicialmente, el primero está
bloqueado y el segundo desbloqueado.

- Cuando todos los threads llegan al primero, se bloquea el segundo y se
  desbloquea el primero.

- Cuando todos llegan al segundo, bloqueamos el primero, lo que hace que puedan
  dar la vuelta sin adelantarse, y después se desbloquea el segundo.

```python
n = cant threads
count = 0
mutex = Semaphore(1)
turnstile = Semaphore(0)
turnstile2 = Semaphore(1)

Thread
while true:
    # rendezvous

    mutex.wait()
        count += 1
        if count == n:
            turnstile2.wait()       # lock the second
            turnstile.signal()      # unlock the first
    mutex.signal()

    turnstile.wait()                # first turnstile
    turnstile.signal()

    # critical point

    mutex.wait()
        count -= 1
        if count == 0:
            turnstile.wait()        # lock the first
            turnstile2.signal()     # unlock the second
    mutex.signal

    turnstile2.wait()               # second turnstile
    turnstile2.signal()
```

Se le llama **two-phase barrier** porque fuerza a todos los threads a esperar
dos veces: cuando todos llegan, y otra para cuando todos terminan de ejecutar
la sección crítica.

Se puede afirmar lo siguiente para ayudar a probar de manera informal que el
programa es correcto:

1. Solo el n-esimo thread puede bloquear o desbloquear los turnstiles
2. Antes de que un thread pueda desbloquear el primer turnstile, tiene que
   cerrar el segundo y viceversa, por lo tanto es imposible para un thread
   adelantarse por más de un turnstile de los demás.

### Queue

Leaders and followers, esperan en dos queues. Cuando llega un lider, chequea si
hay un follower esperando. Si lo hay, procede, y sino queda esperando.

Cuando llega un follower, chequea por un lider y proceeden o esperan de forma
acorde.

```python
leaderQueue = Semaphore(0)
followerQueue = Semaphore(0)

# Leader
followerQueue.signal()
leaderQueue.wait()
dance()

# Follower
leaderQueue.signal()
followerQueue.wait()
dance()
```

#### Exclusive queue

Pero si cada uno puede bailar con solo el que lo trajo?

```python
leaders = followers = 0
mutex = Semaphore(1)
leaderQueue = Semaphore(0)
followerQueue = Semaphore(0)
rendezvous = Semaphore(0)
```

```python
# Leader
mutex.wait()
if followers > 0:
    followers--
    followerQueue.signal()
else:
    leaders++
    mutex.signal()
    leaderQueue.wait()

dance()
rendezvous.wait()
mutex.signal()
```

```python
# Follower
mutex.wait()
if leaders > 0:
    leaders--
    leaderQueue.signal()
else:
    followers++
    mutex.signal()
    followers.wait()

dance()
rendezvous.signal()
```

## Classical synchronization problems

### Producer-consumer

Algunos threads son **productores** y otros **consumidores**.
Los productores crean items y los ponen en una estructura de datos, mientras
que los consumidores los remueven y los procesan.

Cosas a forzar

- El buffer debe tener exclusion mutua
- Si un consumer llega mientras el buffer está vacío, bloquea hasta que un
  producer agrega un nuevo item.

Los producers hacen lo siguiente constantemente

```python
# Producer
event = waitForEvent()
buffer.add(event)
```

Y los consumers

```python
# Consumer
event = buffer.get()
event.process()
```

#### Implementacion

```python
mutex = Semaphore(1)
bufferItems = Semaphore(0) # cantidad de elementos del buffer

# Producer
event = waitForEvent()
mutex.wait()
    buffer.add(event)
mutex.signal()
bufferItems.signal()

# Consumer
bufferItems.wait()
mutex.wait()
    event = buffer.get()
mutex.signal()
event.process()
```

#### Deadlock 4

Si el consumer fuera el siguiente (haciendo wait en el semaforo de items dentro
del mutex), se llega a un deadlock.

```python
# Consumer
mutex.wait()
    bufferItems.wait()
    event = buffer.get()
mutex.signal()
event.process()
```

Por ejemplo, si el buffer esta vacio, un consumer lockea el mutex, y se queda
esperando un item. Luego entra un productor, que se queda esperando el mutex y
nunca puede anunciar que llego un item. Deadlock!

Por lo general, **esperar por un semaforo mientras se tiene un mutex puede**
**tener peligro de deadlock.**

#### Buffer finito

Se tiene el constraint adicional

- Si un productor llega mientras que el buffer está lleno, bloquea hasta que un
  consumidor remueva un item.

```python
mutex = Semaphore(1)
items = Semaphore(0) # cantidad de elementos del buffer
bufferSize = # tamaño del buffer
spaces = Semaphore(bufferSize) # slots libres del buffer

# Producer
event = waitForEvent()

spaces.wait()     # consume un slot libre
mutex.wait()
    buffer.add(event)
mutex.signal()
items.signal()

# Consumer
items.wait()
mutex.wait()
    event = buffer.get()
mutex.signal()
spaces.signal()   # libera un slot
event.process()
```

### Readers-writers problem

Se refiere a cualquier situación en la que una estructura de datos, base de
datos, o file system es leido y modificado por threads concurrentes.

Las constraints son:

1. Cualquier cantidad de lectores puede estar en la sección crítica
   simultáneamente.

2. Los writers deben tener acceso exclusivo a la sección critica.

Un writer no puede entrar a la sección crítica mientras que haya cualquier otro
thread (reader o writer) ahí, y mientras haya writers, no puede entrar ningún
otro thread.

El patron de exclusion es llamado **categorical mutual exclusion**, un thread en
la seccion critica no necesariemente excluye a otros, pero la presencia de una
categoria en la seccion critica exclude a todas las demas.

```python
int readers = 0
mutex = Semaphore(1)
roomEmpty = Semaphore(1)    # 1 si no hay threads en CRIT, 0 sino.

## Reader
mutex.wait()
    readers += 1
    if readers == 1:
        roomEmpty.wait()    # primer reader lockea
mutex.signal()

# CRIT de readers

mutex.wait()
    readers -= 1
    if readers == 0:
        roomEmpty.signal()  # ultimo reader desbloquea
mutex.signal()

## Writer
roomEmpty.wait()
    # CRIT de este writer
roomEmpty.signal()
```

#### Lightswitch

Este patron se conoce como **lightswitch**, ya que el primero en entrar lockea
un semaforo, y el ultimo en salir lo desbloquea. Se puede pensar como que la
primera persona que entra a un cuarto prende la luz, y la ultima que sale la
apaga.

#### Starvation

Es posible que no paren de entrar readers y que nunca le toque a los writers.
Para evitar eso, se puede hacer que cuando llegue un writer, los readers
existentes puedan terminar, pero que no puedan entrar readers adicionales.
