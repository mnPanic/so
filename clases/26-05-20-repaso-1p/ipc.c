#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LINE_SIZE 4

void traducir(const char* line, char* line_traducida) {
    for (size_t i = 0; i < LINE_SIZE; i++) {
        line_traducida[i] = line[i];
    }
}

void hijo(int pipe[2], int pipe_padre[2]) {
    char line[LINE_SIZE];
    read(pipe[0], line, LINE_SIZE);
    char line_traducida[LINE_SIZE];
    traducir(line, line_traducida);
    printf("traducida: '%s'\n", line_traducida);
    write(pipe_padre[1], line_traducida, LINE_SIZE);
}

// n lineas
void traductor(int n, const char* infile, const char* outfile) {
    int fd_in = open(infile, O_RDONLY);
    int fd_out = open(outfile, O_WRONLY | O_CREAT);

    // creo n pipes
    int fds[n][2];
    for (size_t i = 0; i < n; i++ ) {
        pipe(fds[i]);
    }

    int pipe_padre[2];
    pipe(pipe_padre);

    // creo n procesos
    for (size_t i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            hijo(fds[i], pipe_padre);
            exit(0);
        }
    }

    for (size_t i = 0; i < n; i++) {
        char* linea[LINE_SIZE];
        read(fd_in, linea, LINE_SIZE);
        printf("writing line '%s'\n", linea);
        // 0: write
        // 1: read
        write(fds[i][1], linea, LINE_SIZE);
    }

    printf("hola hola hola \n");

    // leo los resultados
    for (size_t i = 0; i < n; i++) {
        char linea[LINE_SIZE];
        read(pipe_padre[0], linea, LINE_SIZE);
        printf("writing translated line '%s'\n", linea);
        write(fd_out, linea, LINE_SIZE);
    }

    close(fd_in);
    close(fd_out);
}

int main() {
    traductor(4, "test.txt", "out.txt");
    return 0;
}