#include <pthread.h>    // POSIX threads
#include <semaphore.h>  // POSIX semaphores
#include <atomic>       // Atomic elements (std C++11)
#include <iostream>
#include <vector>


struct thread_params {
    // tienen que ser punteros porque sino no sería el mismo, se copiaría
    // el semáforo, solo tiene sentido si es el mismo.
    int i;
    sem_t& sem;
};

void* thread_func(void* args) {
    thread_params params = *((thread_params*) args);
    int i = params.i;
    sem_post(&params.sem); // post = signal

    printf("Hola, soy el thread #%d\n", i);
    return nullptr;
}


int main(int argv, char** argc) {
    // tid, es como el pid pero de threads
    pthread_t tid[10];

    sem_t sem;
    sem_init(
        &sem,
        0, // solo se comparten entre threads y no procesos
        0
    );

    int i = 0;
    thread_params params = {0, sem};

    for (int j = 0; j < 10; j++){
        params.i = j;
        pthread_create(&tid[i], NULL, thread_func, &params);
        printf("waiting %d\n", j);
        sem_wait(&sem);
    }

    for (int i = 0; i < 10; i++) {
        // Esperamos que terminen los threads. El equivalente a wait de procesos es
        // join, que permite esperar a que un thread termine.
        pthread_join(tid[i], NULL);
    }

    std::cout << "Se acabó." << std::endl;

    return 0;
}