# Threading

## Proceso

Es un programa en ejecución + la info de contexto que necesita para mantenerlo
que se guarda en la PCB.

Para que los procesos tengan sentido deben tener un *hilo* o *thread* de
ejecución.

- Una **secuencia** de instrucciones de ejecución
- Un **flujo de control** dentro de un programa.
- Una **unidad de scheduling**, y no el proceso.

Un *programa* son instrucciones que no hacen nada, y el hilo es como se va
moviendo la ejecución a lo largo de ese programa.

Proceso == hilo de ejecución? No, un proceso tiene además un montón de recursos
asociados. El hilo es **solamente** como el flujo se mueve a lo largo de sus
instrucciones.

Hasta ahora venimos trabajando con procesos con un solo hilo de ejecución.

Hasta ahora, para lograr *concurrencia*, generamos un proceso nuevo con fork y
dejamos que el scheduler fuera turnando entre uno y el otro. El SO iba a dejar
que ocurran "al mismo tiempo" gracias al scheduling.

## Multithreading

Hay otra manera de lograr concurrencia, tener varios hilos de ejecución dentro
de un mismo proceso. Muchas cosas se comparten entre ellos.

Cada hilo de ejecución necesita

- Su IP
- Sus registros
- Su stack

Por qué usarlos?

- Recursos compartidos
- Comunicación mas sencilla: Ya tengo todo compartido, solo tengo que escribir
  las cosas en lugares donde lo pueda ver.
- Menos *overhead*
  - Menos info de estado
  - Cambios de contexto más baratos
    - Entre varios threads el espacio de memoria se comparte, entonces no hay
      que cambiarlo. El scheduler en general tiene en cuenta los threads.

Cuando un proceso tiene muchos threads, como scheduler veo sus threads y decido
a cual poner en ejecución. Depende de la política de scheduling, uno podría
dividir el quantum entre todos los threads o podría tratarlos como procesos
separados.

### Aplicaciones

En general, aplicaciones que se beneficien de

- Paralelismo
- Interactividad
- Mejor uso del sistema

### Problemas

Trae problemas con sincronización, las condiciones de carrera son la nueva
normalidad. Si programamos siempre asumiendo que todo va a salir mal, todo
debería salir bien. Pero hay que tener cuidado porque por default está todo
compartido entre los distintos procesos.

## Como se lleva a la práctica

Los SOs brindan soporte para los threads, así como uno puede forkear, uno puede
crear nuevos threads. Estos son **threads de kernel**, pero también puede
tener threads en espacio de usuario.

Los threads de usuario no pueden correr al mismo tiempo en distintos cores,
porque el SO no los conoce. Uno tiene control absoluto de como se schedulean.

Los sistemas POSIX ofrecen soporte a través de la interfaz `pthreads`.

> Modelos de threading
> Threads de kernel: modelo 1 a 1, a cada thread de kernel le corresponde 1
> de usuario.
> Uno podría tener un modelo 1 a N, donde 1 de kernel N de usuario,
> o híbrido (por ej. con la JVM).

Primitivas de sincronización

- `<atomic>`: Objetos atómicos
- `<pthread_spin_lock>`: Spinlocks: es un lock en el que uno hace busy waiting
- `<pthread_mutex>`: Mutex , es como un spinlock sin busy waiting, con una
  llamada al sistema para hacer ese lock.
- `<pthread_cond>`: Variables de condición. Se puede usar para las mismas cosas
  que un semáforo pero es distinto.
- `pthread_join`: Un thread puede esperar a que otro termine
- Semaforos POSIX: `pthread` no los brinda. Todas esas cosas solo se pueden usar
  entre threads, pero si quiero sincronizarme con otro proceso, no voy a poder,
  necesito usar semáforos POSIX.
  Si bien los podemos usar entre threads tienen más overhead que el resto.

