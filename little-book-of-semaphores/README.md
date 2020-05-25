# Little book of semaphores

Notas de [*The little book of semaphores*][book]

[book]: http://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf

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

```python
n = cant threads
count = 0
mutex = Semaphore(1)
turnstile = Semaphore(0)

Thread
while true:
    rendezvous

    mutex.wait()
        count += 1
    mutex.signal()

    if count == n: turnstile.signal()

    turnstile.wait()
    turnstile.signal()

    critical point
    count = 0
```
