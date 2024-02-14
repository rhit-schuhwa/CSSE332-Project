#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

#include "kernel/spinlock.h"
#include "kernel/proc.h"

void* thread_func_kill_children(void* args) { 
    int input = *((int*)args);

    sleep(10 * input);

    printf("Thread %d exiting (I shouldn't be here)\n", input);

    exit(0);
}

int test_kill_children(void) {
    int num_threads = 5;

    int args[5];
    int threads[5];

    char** stacks = malloc(num_threads * sizeof(char*));

    for (int i = 0; i < num_threads; i++) {
	stacks[i] = malloc(PGSIZE);
	args[i] = i + 1;
	osthread_create(&threads[i], thread_func_kill_children, &args[i], stacks[i]);
    } 

    printf("Test Kill Children PASSED\n");

    for (int i = 0; i < num_threads; i++) {
	free(stacks[i]);
    }
    free(stacks);
    
    return 0;
}

int write_global_var = 0;

void* thread_func_write_global_vars(void* args) { 
    int* inputs = (int*)args;
    write_global_var += inputs[0];

    sleep(5 * inputs[1]);

    printf("Write Global Var thread %d exiting\n", inputs[1]);

    exit(0);
}

int test_write_global_vars(void) {
    int num_threads = 5;

    int args[10];
    int threads[5];

    char** stacks = malloc(num_threads * sizeof(char*));

    for (int i = 0; i < num_threads; i++) {
	stacks[i] = malloc(PGSIZE);
	args[2 * i] = 2;
	args[2 * i + 1] = i;
	osthread_create(&threads[i], thread_func_write_global_vars, args + 2 * i, stacks[i]);
    }

    for (int i = 0; i < num_threads; i++) {
	osthread_join(threads[i], 0);
    }

    if (write_global_var != 2 * num_threads) {
	printf("Test Write Global Vars FAILED (write_gloal_var = %d, expected write_global_vars = %d)\n", write_global_var, 2 * num_threads);
    } else {
	printf("Test Write Global Vars PASSED\n");
    }

    for (int i = 0; i < num_threads; i++) {
	free(stacks[i]);
    }
    free(stacks);
    
    return 0;
}

int read_global_var = 15;

void* thread_func_read_global_vars(void* args) { 
    int input = *((int*)args);
    if (input * read_global_var != 15 * 17) {
	printf("Test Read Global Vars FAILED (input * read_global_vars = %d, expected input * read_global_vars = %d)\n", input * read_global_var, 15 * 17);
    } else {
	printf("Test Read Global Vars PASSED\n");
    }
    exit(0);
}

int test_read_global_vars(void) {
    osthread thread;

    int args[] = {17};
    char* stack = malloc(PGSIZE);

    osthread_create(&thread, thread_func_read_global_vars, &args, stack); 

    osthread_join(thread, 0);

    free(stack);
    return 0;
}

void* thread_func_basic_args(void* args) { 
    int* inputs = (int*)args;
    if (inputs[0] != 14 || inputs[1] != 43) {
	printf("Test Basic Args FAILED (args = {%d, %d}, expected args = {14, 43})\n", inputs[0], inputs[1]);
    } else {
	printf("Test Basic Args PASSED\n");
    }
    exit(0);
}

int test_basic_args(void) {
    osthread thread;

    int args[] = {14, 43};
    char* stack = malloc(PGSIZE);

    osthread_create(&thread, thread_func_basic_args, &args, stack); 

    osthread_join(thread, 0);

    free(stack);
    return 0;
}

void* thread_func_join(void* args) {
    sleep(10);
    printf("Exiting thread for Test Join\n");
    exit(0);
}

int test_join(void) {
    osthread thread;

    char* stack = malloc(PGSIZE);

    osthread_create(&thread, thread_func_join, 0, stack); 

    osthread_join(thread, 0);
    
    printf("Exiting Test Join\n");

    free(stack);
    return 0;
}

int main(int argc, char** argv) {
    test_join();
    test_basic_args();
    test_read_global_vars();
    test_write_global_vars();

    test_kill_children(); // THIS HAS TO GO LAST PLEASE DO NOT TOUCH!!!!!
    exit(0);
}
