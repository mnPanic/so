/*
Escribir un programa que construya un arbol de procesos que represente la
siguiente genealogía

Abraham es padre de homero,
    homero es padre de bart
    homero es padre de lisa
    homero es padre de maggie

Cada proceso imprime por pantalla el nombre de la persona que representa
*/

#include <unistd.h>
#include <stdio.h>

int main(void) {
    pid_t pid = fork();

    if(pid != 0) {
        // abraham
        printf("abraham\n");
    } else {
        // homero
        printf("homero\n");
        pid = fork();
        if (pid != 0) {
            // homero
            pid = fork();
            if (pid != 0) {
                // homero
                pid = fork();
                if(pid != 0) {
                    // homero
                } else {
                    printf("bart\n");
                    return 0;
                }
            } else {
                printf("maggie\n");
                return 0;
            }
        } else {
            printf("lisa\n");
            return 0;
        }
    }
}
