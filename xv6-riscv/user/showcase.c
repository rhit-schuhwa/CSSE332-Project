#include "osthread.h"

struct spinlock lock;

void* thread_func(void* args) {
    int input = *((int*)args);
    printf("Thread %d started\n", input);
    for (int i = 0; i < 5; i++) {
        printf("Thread %d is working\n", input);
        sleep(1);
    }

    printf("Thread %d finished\n", input);
    exit(0);
}

int main() {
    int num_threads = 5;
    int args[num_threads];
    int threads[num_threads];
    for (int i = 0; i < num_threads; i++) {
        args[i] = i;
        osthread_create(&threads[i], thread_func, &args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        osthread_join(threads[i], 0);
    }

    printf("\nAll threads finished\n");

    exit(0);
}

