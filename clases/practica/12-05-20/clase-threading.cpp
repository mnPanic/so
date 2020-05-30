#include <pthread.h>    // POSIX threads
#include <semaphore.h>  // POSIX semaphores
#include <atomic>       // Atomic elements (std C++11)
#include <iostream>
#include <vector>

void* thread_func(void* args) {
    int i = *((int*) args);
    printf("Hola, soy el thread #%d\n", i);
    return nullptr;
}

int main(int argv, char** argc) {
    // tid, es como el pid pero de threads
    pthread_t tid[10];
    int numeros[10];

    for (int i = 0; i < 10; i++){
        // man pthread_create
        // Hay que pasarle un puntero a la función que va a ejecutar y luego
        // los argumentos.
        numeros[i] = i;
        pthread_create(&tid[i], NULL, thread_func, &numeros[i]);
    }

    for (int i = 0; i < 10; i++) {
        // Esperamos que terminen los threads. El equivalente a wait de procesos es
        // join, que permite esperar a que un thread termine.
        pthread_join(tid[i], NULL);
    }

    std::cout << "Se acabó." << std::endl;

    return 0;
}