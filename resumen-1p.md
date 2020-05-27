# Resumen para el primer parcial de SO

## IPC - Inter Process Communication

### FDs

Cada proceso tiene en su PCB una tabla con referencias a los archivos que tiene
abiertos. Un **file descriptor** es un indice de esa tabla.

Distintas entradas de file descriptor pueden apuntar al mismo archivo, pero
son *instancias de apertura* distintas.

### Comunicacion

Cada proceso tiene definida una entrada y salida estandar, y puede abstraerse
de donde y hacia donde se está escribiendo.

Por lo general los procesos esperan tener abiertos 3 **file descriptors**
(las entries 0, 1 y 2 de la tabla)

- 0 = stdin
- 1 = stdout
- 2 = stderr

### Interaccion

Se heredan del padre cuando se crea el proceso.

- `ssize_t read(int fd, void *buf, size_t count);`
- `ssize_t write(int fd, const void *buf, size_t count);`
  
donde `fd` es el file descriptor.
Ambas devuelven la cantidad de bytes que devuelve, -1 en caso de error.

Son **bloqueantes** por defecto, es decir, por ej. con el read hasta que no
encuentre información disponible se queda esperando.

### dup2

```bash
echo "Es jueves y mi PCB lo sabe" > archivo.txt
```

- Se llama al programa echo, que escribe su parámetro por **stdout**
- Con > se le indica a la consola que **stdout** se redirija a `archivo.txt`
- Cómo? Abre archivo.txt y hace que la entry de stdout apunte a el

La función `dup2(int oldfd, int newfd)` pisa el file descriptor en `newfd` el
contenido que está en `oldfd` (`man dup2`)

```text
$ man dup

    ...

    #include <unistd.h>

    int dup(int oldfd);
    int dup2(int oldfd, int newfd);

    ...

    The  dup()  system  call  creates  a copy of the file descriptor oldfd,
    using the lowest-numbered unused file descriptor for the  new  descrip‐
    tor.

    After a successful return, the old and new file descriptors may be used
    interchangeably.  They refer to the same  open  file  description  (see
    open(2)) and thus share file offset and file status flags; for example,
    if the file offset is modified by using lseek(2) on  one  of  the  file
    descriptors, the offset is also changed for the other.

    The two file descriptors do not share file descriptor flags (the close-
    on-exec flag).  The close-on-exec flag (FD_CLOEXEC; see  fcntl(2))  for
    the duplicate descriptor is off.

dup2()
    The  dup2() system call performs the same task as dup(), but instead of
    using the lowest-numbered unused file  descriptor,  it  uses  the  file
    descriptor number specified in newfd.  If the file descriptor newfd was
    previously open, it is silently closed before being reused.
```

### Pipes

```bash
echo "sistemas" | wc -c
```

Los pipes tienen dos extremos, uno de lectura y uno de escritura, y se pueden
crear mediante una syscall.

```c
int pipe(int pipefd[2])
```

- `pipefd[0]`: fd que apunta al extremo del pipe en el cual se **lee**
- `pipefd[1]`: fd del extremo en el que se **escribe**

## Sincronizacion

### Primitivas de sincronizacion

Semaforos, atomic

### Propiedades

#### Tipos

Contraejemplo: Sucesión de pasos que muestra una ejecución del sistema que viole
cierta propiedad.

- **safety**
  - Intuición: Nada malo sucede
  - Nunca pasan cosas que no queremos que pasen"
  - Ejemplos: Mutex, ausencia de deadlock, no perdida de mensajes, etc.
  - Definición: Tienen un *contraejemplo finito*
- **progreso** o **liveness**
  - Intuición: En algun momento algo bueno si va a suceder
  - El sistema progresa, suceden cosas, el sistema no se queda bloqueado.
  - Ejemplos: "Si se presiona el botón de stop, el tren frena", no inanición.
  - Definición: Los contraejemplos no son finitos (una secuencia infinita de
    pasos).
- **fairness**
  - Intuición: Los procesos reciben su turno con infinita frecuencia.
  - "No se van a dar escenarios poco realistas en donde alguien es postergado
    para siempre"

#### Modelo de proceso

TODO

#### WAIT-FREEDOM

Todo proceso que intenta acceder a la sección crítica, en algún momento lo
logra, cada vez que lo intenta.

Para todo proceso, si está en un estado TRY, habrá un momento posterior en el
que estará en CRIT.

Intuicion: Libre de procesos que esperan para siempre.

#### FAIRNESS - Equanimidad

.

#### EXCL - Exclusión Mutua

Para toda ejecución y estado, no puede haber más de un proceso i que esté en el
mismo punto en el tiempo en CRIT.

#### LOCK-FREEDOM - Progreso

Para toda ejecución y estado, si hay un proceso en TRY, y ninguno en CRIT,
entonces hay un estado posterior tal que algún proceso está en CRIT.

#### DEADLOCK/LOCKOUT/STARVATION-FREEDOM - Progreso global dependiente

IN: Si está en try, en algún momento está en crit
OUT: Si está en crit, en algún momento sale

Para toda ejecución, si para todo estado y proceso i que está en crit, existe un
estado tal que sale, entonces para todo estado posterior y todo proceso
diferente, si intenta de entrar existe un estado para el cual entra.

Que todos salgan implican que todos van a entrar en algún momento.

#### WAIT-FREEDOM - Progreso global absoluto

Todo proceso entra a la sección crítica

$WAIT-FREEDOM \equiv \forall i \square IN(i)$

## Memoria

TODO