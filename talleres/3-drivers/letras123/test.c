// open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// read & write
#include <unistd.h>

// print
#include <stdio.h>

int main() {
    int dev = open("/dev/letras123", O_RDWR);
    if (dev == -1) {
        printf("err\n");
    }

    char w[1] = {"a"};
    write(dev, &w[0], 1);

    char buf[5];
    buf[4] = '\0';
    read(dev, buf, 4);

    printf("got: %s\n", buf);
    printf("finished\n");
}