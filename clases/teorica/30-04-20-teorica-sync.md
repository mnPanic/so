# Sincronización entre procesos 1 - Montivación y herramientas

## Repaso

El scheduling es la tarea que decide a quien le toca ejecutar en cada momento.
> A quien voy a poner a ajecutar ahora y por cuanto tiempo

La manera de decidir es mediante una política, las cuales siempre priorizan
ciertos objetivos en detrimento de otros. Y depende de lo que queremos que haga
el SO cual elegir. Incluso hay ciertos SOs que brindan más de un scheduler
según la tarea que se quiera realizar.

## Ideas

Este es uno de los puntos *centrales*. Los SOs no solo son para gente "fierrera"
Conocer bien como funciona el SO nos hace mejores programadores.

Entender el equilibrio entre muchos procesos a la vez, y que lo hagan sin
estorbarse, de forma ordenada.

**Contención y concurrencia**: de forma correcta y con buen rendimiento.

## Ejemplo

p. 5

**interleaving**: Manera en la cual se van entremando las instrucciones

!!!! Un programa debería andar bien para cualquier tipo de scheduling, por ej.
ante un scheding adversarial, aun en ese caso debería funcionar bien.
!!!! Si no se corresponde con alguna de las posibles ejecuciones secuenciales,
entonces es una ejecución inválida.

Lo correcto sería que la ejecución del programa en paralelo de alguno de los
resultados posibles de haberlo ejecutado de forma secuencial.
Por ej, P1, P2 o P2, P1.

## Race conditions

**Race condition**: o *condicion de carrera* es lo que ocurrió. Es algo que
sucede cada vez que pensamos en un programa paralelo.
Es una ejecución inválida producto de la manera en la que se dió el
interleaving.

Si el resultado que obtengo varía según el timing de la ejecución: race
condition.

Se llama *de carrera* porque el resultado es según quien se apura más y llega
primero.

### CRIT

Buscamos la exclusión mutua, es decir, hay una parte del programa que si acá
en el medio se mete algo, la cagamos.

**Sección crítica** (CRIT): un cacho de código que se ejecute solo en
este proceso.

- Solo hay un único proceso en CRIT
- Todo proceso que está esperando para entrar a CRIT va a entrar.
  (no me dejan boyando para siempre).
- Ningún proceso fuera de CRIT puede bloquear a otro.

A nivel de código se agregan dos primitivas: acá arranca y termina una sección
CRIT.

Para hacer una implementación,

- Si yo soy el único proceso, me hace ejecutar.
- Si hay otros ejecutando, se encarga de que espere el turno.
- La instrucción de salida de cierta forma hace que "pase el siguiente"

```c
entrar()
...
fondo = fondo + var
...
salir()
```

#### Suspender interrupciones

Se pueden suspender todas las interrupciones, la cual **no** es una alternativa
ya que afecta *mucho* al rendimiento.

#### Lock

Una variable booleana ***lock***, pongo un 1 cuando quiero entrar y un 0 cuando
quiero salir.

No funciona:

```c
void entrar() {
    if lock == 0 {
    // puede pasar que justo acá el scheduler cambie, y otro proceso también
    // arranca a ejecutar
        lock == 1
        EJECUTAR
    }
}
```

#### TAS (Hardware)

Se soluciona con ayuda del hardware.

Se agrega una instrucción al ISA que de manera *atómica* permite poner una var
entera en el valor 1.

Atómica: de una manera tal que no puede ser interrumpida por otro proceso.

Se suele llamar **TestAndSet** o **TAS**.

No solo pone el valor 1, sino que devuelve el valor anterior.

```c
// pseudocódigo
bool TestAndSet(bool *dest) {
    bool res = *dest;
    *dest = TRUE;
    return res;
}
```

Una invocación de ejemplo

```c
bool lock; // compartida

void main(void) {
    while(TRUE) {
        // Si da true, ya estaba lockeado, no hago nada.
        while(TestAndSet(&lock)); // greedy

        // Me tocó, estoy en CRIT. Hago lo que haga falta.
        ...

        // Salgo de la sección crítica.
        // No es necesario hacer TestAndSet ni nada porque no hay forma de que
        // poner un 0 en una variable tenga varios interleavings posible.
        // Es una operación naturalmente atómica. (ya que es una sola inst.)
        lock = FALSE;

        // Acá puedo hacer las cosas no críticas
    }
}
```

Es **correcto** pero **no eficiente**.

Es muy *greedy* y agresiva, todo el tiempo preguntando si el lock está liberado.
Gasta un montón de procesador.

A esto se lo llama *busy waiting*, estoy esperando pero todo el tiempo fijandome
si está listo. Esto perjudica al resto de los procesos.

Una buena idea sería en el medio poner a correr a otro proceso.

Sleep? No es trivial elegir por cuanto tiempo.

De alguna forma decirle al SO que avise cuando `lock == 0`, para ahí continuar
la ejecución.

#### Dijkstra (semáforos)

Conceptualiza el problema: un escenario en el que hay un productor y un
consumidor de información, que se comunican a través de un *buffer* fijo, en
el cual el productor pone elementos y el consumidor quiere sacar.

- No queremos que el consumidor intente de sacar cuando no hay nada
- No queremos que el productor quiera escribir cuando no hay lugar

Ambos quieren actualizar las mismas variables.

Primitivas

- `sleep()`: Despertame cuando estén dadas las condiciones para que siga
- `wakeup()`: Despertá a algún proceso porque generé las condiciones para que
  alguno siga.

```c
// consumidor
if cant == 0 {
    sleep();
}

// productor
agregar(item, buffer);
cant++;
wakeup();
```

*Lost wakeup problem*
{posible interleaving ej diapo 20}

Djistra inventó los **semáforos** para solucionar este problema.
> E.W Dijkstra, *Cooperating Sequential Processes*. Technical Report EWD-123

Un semáforo es una variable entera que cumple:

- La puedo inicializar en cualquier valor
- Solo se puede manipular mediante dos operaciones:
  - `wait()` (o `P()`, `down()`)
  - `signal()` (o `V()`, `up()`)

```c
wait(s)
    while(s <= 0)
        // si no hay slot disponible, me voy a dormir
        dormir();

    // si hay un slot disponible, lo tomo

    s--;
```

```c
signal(s)
    s++;
    if (alguien esperando por s)
        despierto a alguno
```

Ambas se implementan a nivel de kernel, sleep: el proceso pasa a estar en
**bloqueado** en vez de ready.

El semáforo no se puede implementar a nivel de usuario.

- **slot**: cuantos procesos pueden estar ejecutando la sección crítica
  a la vez. Lo normal es 1, pero podría haber más de uno.

Un tipo especial de semáforo es un **mutex**, de **mutual exclusion**, es un
semáforo binario.

[código diapo 23]

### Deadlocks

Es uno de los problemas de la concurrencia.

El proceso A se queda esperando que suceda algo que solo B puede provocar.
Pero B a su vez está esperando algo de A.

```c
proceso A:
    wait(s1);
    signal(s2);

proceso B:
    wait(s2);
    signal(s1);
```

## Lenguajes modernos

Como evitamos tener un deadlock? Ideas que se basan en las secciones anterioes

### Registro / Objeto atómico

Es conceptual, tiene que ser a nivel de hardware. Provee dos primitivas

- `getAndSet`
- `testAndSet`

diapo 27

### TASLock (spin locks)

TASLock o *spin lock*, que hace busy waiting.

diapo 28.

Los kernels lo usan mucho, es mucho más simple que los semáforos.
En algunos casos tiene sentido hacer esto en vez de semáforos, si por ejemplo
mi sección crítica es chiquita.
Perder tiempo a veces es lo más barato.

### TTASLock (local spinning)

diapo 31

idea: probar antes de mandarse con el TAS, también llamado *local spinning*.

Local spinning es más eficiente, se comporta mejor a medida que crece la
cantidad de procesos.

En el momento que la veo en 0, parece que la liberaron, y ahí hago el commit
al TestAndSet. Muchas de las veces que iba a perder pierdo de la forma
más barata, y solo ejecuto la instrucción posta y más cara.

El `get` no es atómico. y `testAndSet` si. La atomicidad frena la concurrencia.
Varios procesos pueden hacer `get` a la vez.
También es más rápido porque lees del cache.

## Condiciones de Coffman

Están mal

Da las condiciones necesarias para un deadlock, si pruebo que mi código no
cumple con una de ellas, va a estar libre de deadlocks.

1. (recursos únicos) hay recursos que no pueden ser compartidos
2. hay procesos que teniendo un recurso solicitan otro
3. que no haya una manera de sacarle los recursos
4. Espera circular

Esto es así en ciertos modelos, pero no en otros. Por ejemplo, auto deadlock.
