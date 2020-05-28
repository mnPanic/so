#include <stdio.h>  // printf
#include <unistd.h> // read


void child(int pipefds[2]) {
    char buf[5];
    read(pipefds[0], buf, 5);
    printf("read: %s\n", buf);

    char wbuf[5] = "okas\0";
    write(pipefds[1], wbuf, 5);
}

void parent(int pipefds[2]) {
    // write
    char buf[5] = "hola\0";
    write(pipefds[1], buf, 5);

    // no se sincroniza asi pero ya fue
    sleep(2);

    read(pipefds[0], buf, 5);
    printf("read: %s\n", buf);
}

int main() {
    int pipefds[2];
    pipe(pipefds);

    pid_t pid = fork();
    if (pid == 0) {
        child(pipefds);
    } else {
        parent(pipefds);
    }
}
