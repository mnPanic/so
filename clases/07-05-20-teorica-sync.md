# Sincronización entre procesos 3 - Razonamiento y problemas clásicos (2)

## Repaso

- **fairness**: muchas veces la pedimos como precondición. Estoy jugando en
  contra del scheduler, con uno suficientemente malo alguno de mis procesos
  nunca reciba su turno.
- Exclusión mutua
- **LOCK-FREEDOM** (*Progreso del sistema*): Alguno avanza
- **STARVATION-FREEDOM**: No hay inanición, si todos se portan bien todos van
  a poder entrar. (*Progreso global dependiente*)
- **WAIT-FREEDOM**: Idea máxima, pase lo que pase todos van a poder entrar
  a la sección crítica. (*Progreso global absoluto*).

Hoy vemos más problemas

## Problemas

### Livelock

*El primo olvidado de deadlock*. En un deadlock el sistema se traba, en un
livelock el sistema se retroalimenta y no para nunca.

> ej diapo 4

### SCM

Sección crítica de a M <= N.

Hasta ahora queríamos que en la sección crítica haya solo uno, pero hay casos
en donde nos bancamos más de uno pero no queremos excedernos de N.

> prop diapo 5

```c
semaphore sem = M;
proc P(i) {
    // TRY
    sem.wait();
    // CRIT
    sc(i);
    // EXIT
    sem.signal();
}
```

En general lo que determina esta cantidad es el *problema*

### Lectores / Escritores

Muy común en base de datos.

- La idea es que hay una variable compartida
- Los escritores necesitan acceso exclusivo
- Los lectores pueden leer simultáneamente.

#### Prop SWMR (Single Writer/Multiple Readers)

> prop diapo 6

- solo hay un escritor en la sección crítica.
- si hay un lector en la sección critica, el resto que están son lectores.

Se viola STARVATION-FREEDOM

Tarea: solución

### Filósofos que cenan

5 filósofos en una mesa circular.

5 platos de fideos y 1 tenedor en cada plato, para comer necesitan 2 tenedores

```c
proc Filosofo(i) {
    while(true)
    pensar()                    // REM
    tomar_tenedores(i)          // TRY
    comer()                     // CRIT
    soltar_tenedores(i)         // EXIT
}
```

La solucion trivial siempre es secuencializar, pero la idea es mantener
paralelismo

Problema: programar TRY y EXIT satisfaciento

- EXCL-FORK: no tiene los tenedores más de uno
- WAIT-FREEDOM: no hay deadlock
- STARVATION-FREEDOM: no hay inanición
- EAT: mas de un filosof esté comiendo a la vez (variante de SCM)

> no existe solución simétrica, en las que todos los filósofos hacen lo mismo
> lynch dist. algorithms cap 11

Tarea: pensar en soluciones, buscar las ya existentes

### Barbero

- En una peluquería hay un único peluquero.
- Dos salas
  - Espera: N sillas
  - Corte de pelo: unica silla
- Cuando no hay clientes, duerme la siesta. "no gasta energía"
- Cuando entra un cliente
  - Si no hay lugar de espera se va
  - Si está dormido lo despierta

> Tengo pedidos que llegan, puedo atender de a uno por vez, y tengo una cola de
> espera. Lo puedo atender o ponerlo en la cola, que también tiene un tamaño.
> Quiero un algoritmo que si entra uno solo lo atienda, y si hay más lo encole,
> hasta N.

Tarea: Formalizar las props a garantizar.

## Jerarquía de registros atómicos

### Registros RW

> imagen diapo 17

Características

- Si `read` y `write` no se solapan
  - `read` devuelve el **último** valor escrito

> Pero si hay varios leyendo y escribiendo a la vez, y se solapan?

- Si `read` y `write` se solapan
  - *Safe*: `read` devuelve **cualquier** valor.
  - *Regular*: `read` devuelve **algún** valor de los que se escribieron en
    algún momento. Algo que en algún momento fue el último escrito.
  - *Atomic*: `read` devuelve un valor **consistente** con una serialización.
    Es el valor que hubieses obtenido en algunas de las posibles serializaciones
    Si hay varios procesos que están físicamente en el *mismo* segundo
    trabajando a la vez, no hay un resultado posta, porque están trabajando a
    la vez.

### Teorema (Burns & Lynch)

No se puede garantizar **EXCL** y **LOCK-FREEDOM** con menos de n registros RW.

## Calidad

- Que tan buenos son los registros RW?
- Que tan bueno es TAS?

### Problema del consenso

- *"Pueden n procesos (concurrentes) acordar sobre un estado booleano?"*

- Valores `V = {0, 1}`
- **inicio** Todos proceso i empieza con init(i) € V
- **decisión** Todo proceso i decide un valor, `decide(i) € V`

El problema requiere:

- **acuerdo** para todo i!= j decide(i) = decide(j)
- **validez** existe i tal que init(i) = decide(i)
- **terminación** todo i decide en un número finito de transiciones

Teo (Herlyhy, Lynch): usando solo registros RW atómicos para un n arbitrario
**no** se puede garantizar.

#### Consensus number

Jerarquía de mecanismos de sincronización

**Consensus number**: cantidad de procesos para los que resuelve consenso.

Si se la cantidad de procesos a priori,

- Registros RW atómicos = 1, solo usando registros atomicos, para un solo proc.
- Colas, pilas = 2, para hasta dos procesos
- TAS (getAndSet()) = 2

Existe alguno con consensus number mayor?

CAS: CompareAndSwap / CompareAndSet

Tiene un número de consenso mayor.

Está en hardware

`compareAndSwap()` cmpxch en intel x86

- compareAndSwap tiene consensus number **infinito**.

