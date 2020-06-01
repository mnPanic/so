#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(){
    // Creo el pipe para comunicarlos
    int pipe_fds[2];
    pipe(pipe_fds);

    // armo ls -al
    pid_t pid_ls = fork();
    if(pid_ls == 0){
        // mapeo stdout al pipe
        dup2(pipe_fds[1], STDOUT_FILENO);

        // cierro los pipes
        close(pipe_fds[0]);
        close(pipe_fds[1]);

        // llamo al programa
        char* args[3] = {"ls", "-al", NULL};
        execv("/bin/ls", args);
    }

    // armo wc -l
    pid_t pid_wc = fork();
    if(pid_wc == 0){
        // mapeo stdin al pipe
        dup2(pipe_fds[0], STDIN_FILENO);

        // cierro los pipes
        close(pipe_fds[0]);
        close(pipe_fds[1]);

        // llamo al programa
        char* args[3] = {"wc", "-l", NULL};
        execv("usr/bin/wc", args);
    }

    // cierro los pipes
    close(pipe_fds[0]);
    close(pipe_fds[1]);

    // espero a que terminen
    waitpid(pid_ls, NULL, 0);
    waitpid(pid_wc, NULL, 0);
}