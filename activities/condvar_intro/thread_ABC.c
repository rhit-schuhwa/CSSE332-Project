
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


//         INSTRUCTION
// Use condition variables to make  A, B, C print out in order.
// HINT: You need more than one condition variables

pthread_cond_t c = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int flagA = 0, flagB = 0;

void* thread_func_A(void* arg) {
    sleep(3);

    printf("A\n");

    pthread_mutex_lock(&m);
    flagA = 1;
    pthread_cond_broadcast(&c);
    pthread_mutex_unlock(&m);

    return NULL;
}

void* thread_func_B(void* arg) {
    sleep(2);

    pthread_mutex_lock(&m);
    while (!flagA) {
	pthread_cond_wait(&c, &m);
    }
    pthread_mutex_unlock(&m);
    
    printf("B\n");

    pthread_mutex_lock(&m);
    flagB = 1;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
    
    return NULL;
}
void* thread_func_C(void* arg) {
    pthread_mutex_lock(&m);
    while (!flagB) {
	pthread_cond_wait(&c, &m);
    }
    pthread_mutex_unlock(&m);
    
    printf("C\n");

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t tA, tB, tC;

    pthread_create(&tC, NULL, thread_func_C, NULL);
    pthread_create(&tB, NULL, thread_func_B, NULL);
    pthread_create(&tA, NULL, thread_func_A, NULL);

    // join waits for the threads to finish
    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    pthread_join(tC, NULL);
    return 0;
}
