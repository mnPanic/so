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
#include <stdlib.h>

int main(void) {
    // Este proceso deberia ser abraham
    pid_t pid;
    printf("Soy Abraham\n");

    pid = fork();
    if (pid == 0) {
        printf("Soy Homero\n");
        pid = fork();
        if (pid == 0) {
            // Bart
            printf("Soy Bart\n");
            exit(0);
        }
        pid = fork();
        if (pid == 0) {
            // Lisa
            printf("Soy Lisa\n");
            exit(0);
        }
        pid = fork();
        if (pid == 0) {
            // Maggie
            printf("Soy Maggie\n");
            exit(0);
        }
        exit(0);
    }
    return 0;
}
