/*                                  IPC

Resolucion del ejercicio 1 y 2 del primer parcial de SO, de comunicacion entre
procesos (IPC).

Para explicar el razonamiento, se dejan comentarios en el codigo y al fondo se
agrega un analisis de las opciones brindadas por los docentes.
*/

int main(int argc, char **argv){

    int fd[2];      // fds del pipe
    int leer;       // fd de lectura del pipe
    int escribir;   // fd de escrita del pipe
    int estatus;    // status de waitpid
    pid_t pid;      // para almacenar el resultado de fork()
    int i = 1;      // el proceso actual
    int N, t[1];    // N: cant procesos
                    // t: buffer de lectura y escritura

    if (argc != 3) {
        printf("Uso: nombre_program <N> <c> \n");
        // N es la cantidad de procesos a crear
        // c es un caracter que se le envia al primer proceso
        exit(0);
    }

    N = atoi(argv[1]);
    t[0] = atoi(argv[2]);

    // Crea un pipe y escribe el segundo argumento del programa, un caracter
    if (pipe(fd) < 0) {
        perror("Error de creación\n");
        exit(0);
    }

    leer = fd[0];
    escribir = fd[1];
    write(escribir, t, 1); 

    // Lanza N procesos
    for(i = 0; i < N; i++) {
        // Si no vamos a crear el ultimo proceso
        if (i != N-1)   
            // Se crea un nuevo pipe
            if (pipe(fd) < 0) {
                perror("Error de creación\n");
                exit(0);
            }
        // Se forkea la ejecucion, creando un nuevo proceso.
        pid = fork();
        // pid es
        // * -1 si hubo un error
        // * > 0 si es el padre, y pid es el nombre del proceso hijo
        // * 0 si es el hijo.
        switch(pid) {
            case -1: /* error */
                perror("Error en el fork \n");
                exit(0);
            case 0: 
                // Hijo
                // escribir y leer son los anteriores.
                // En caso del primer proceso, es el primer pipe que abre el
                // padre, al cual se le envio el caracter que se paso por arg.
                if (i != N-1) {
                    // Si no es el ultimo proceso...
                    close(escribir);        // cierra el fd del extremo de write del pipe
                    read(leer, t, 1);       // lee el caracter que le mandaron
                    close(leer);            // cierra el fd del extremo de read del pipe
                    t[0]++;                 // incrementa el caracter a enviar. Es el siguiente.
                    close(fd[0]);
                    write(fd[1], t, 1);
                    close(fd[1]); 
                }
                break;
            default:
                // Padre
                if (i == N-1) {
                    // Si se creo el ultimo proceso, entonces ya se leyo el
                    // caracter
                    read(leer, t, 1);
                    // se cierran los pipes
                    close(leer); 
                    close(escribir); 
                    exit(0);
                }
                else { 
                    // Espera que termine el proceso hijo
                    pid_t pid2; // agregado por mi
                    if ((pid2 = waitpid(pid, &estatus, 0)) == -1)
                        perror("wait() error");
                    else {
                        // una vez que termino, cierra los pipes
                        close(leer); 
                        close(escribir);
                        // cambia el extremo de lectura y escritura del pipe
                        // para que el proximo lea de aqui
                        leer = fd[0]; 
                        escribir = fd[1];
                    }
                }
        }

        if (pid==0)
            // si es el hijo, no sigue iterando
            break;
        
        // si es el padre, sigue iterando
    }

    // Este proceso basicamente lanza N subprocesos, los cuales se comunican
    // entre si con pipes (mediante la ayuda del padre) e incrementan un valor
    // 1 a 1. El padre finalmente lee el resultado.

    // Analisis de cada opcion:
    // a. El padre no le manda nada a ningun hijo (no hace write())
    // b. Esta
    // c. Ni vimos sockets en la practica
    // d. No crea N+1 procesos, sino N.
    exit(0);
}
