# IPC - Inter Process Communication

- Redirección de E/S
- Comunicación via pipes.

## File descriptors

Son indices de una tabla que de alguna forma referencian a archivos abiertos
por un proceso. Cada entrada en la tabla referencia a un archivo

En UNIX, cada proceso viene con su propia tabla en su PCB.

[imagen diapo 3]

Distintas entradas de file descriptor pueden apuntar al mismo archivo, pero
son *instancias de apertura* distintas.

## Modelando flujo de comunicación

[imagen diapo 4]

Cada proceso tiene definida una entrada y salida estandar, y puede abstraerse
de donde y hacia donde se está escribiendo.

En general, la entrada estandar está asociada al teclado, y la salida estándar
a la consola.

    teclado     proceso     terminal
            ->          ->
            in          out

La entrada y salida están mapeadas a archivos, entonces escribir y leer no es
más que leer de archivos.

Otros esquemas,

    ls  -out-> term
        1

**En UNIX**: el teclado y la pantalla se modelan como un archivo

Por lo general los procesos esperan tener abiertos 3 **file descriptors**
(las entries 0, 1 y 2 de la tabla)

- 0 = stdin
- 1 = stdout
- 2 = stderr

Estos se heredan del padre.

## Como escribo

- `ssize_t read(int fd, void *buf, size_t count);`
- `ssize_t write(int fd, const void *buf, size_t count);`
  
donde `fd` es el file descriptor.
Ambas devuelven la cantidad de bytes que devuelve, -1 en caso de error.

Son **bloqueantes** por defecto, es decir, por ej. con el read hasta que no
encuentre información disponible se queda esperando.
Hay ciertos flags que permiten configurar para que no lo sea (man algo)

## Redirección

```bash
echo "Es jueves y mi PCB lo sabe" > archivo.txt
```

- Se llama al programa echo, que escribe su parámetro por **stdout**
- Con > se le indica a la consola que **stdout** se redirija a `archivo.txt`
- Cómo? Abre archivo.txt y hace que la entry de stdout apunte a el

La función `dup2(int oldfd, int newfd)` pisa el file descriptor en `newfd` el
contenido que está en `oldfd` (`man dup2`)

[imagen diapo 8]

En c

## Pipes

Ej

```bash
echo "sistemas" | wc -c
```

`wc -c`: cuenta la cantidad de bytes

Conecta el **stdout** de echo al **stdin** del otro programa. A diferencia de
`>`, se comunica el output al input.

Es una forma de que dos procesos puedan mandarse mensajes.

[figura diapo 10]

El pipe en el fondo identifica un archivo, una especie de buffer. Y tiene un
file descriptor que lo apunta. Y nos interesa diferenciar el extremo de lectura
que el de escritura, ya que va de izquierda a derecha.

Hay dos fds, uno que apunta al extremo de lectura y otro al de escritura.

- echo tiene su stdout mapeado al stdin del pipe,
- wc tiene su stdin mapeado al stdout del pipe

Son equivalentes?

- `a && b` ejecuta primero `a` y después `b`
- `a < fd_b`: redirecciona el stdin de a a fd_b

```bash
echo "sistemas" | wc -c
echo "sistemas" > temp.txt && wc -c < temp.txt
```

Son equivalentes en cuanto a resultado final, pero en el primero hay streaming
y en el segundo se pierde.

### Uso

Los pipes funcionan sobre unix pero no necesariamente es así sobre otras distros

syscall

```c
int pipe(int pipefd[2])
```

- `pipefd[0]`: fd que apunta al extremo del pipe en el cual se **lee**
- `pipefd[1]`: fd del extremo en el que se **escribe**

`perror`: escribe en stderr

El SO provee una forma segura de leer y escribir de forma concurrente. El orden
lo define el SO pero no es que queda cortado.