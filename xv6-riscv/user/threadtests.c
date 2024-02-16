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

int** global_stack_addresses;

void* thread_func_check_stack(void* args) {
    int input = *((int*)args);
    if (input != 4)
        sleep(5 * (input + 1));

    // Allocate a variable on the stack and store its address in a global array
    int stack_var = input;
    global_stack_addresses[input] = &stack_var;

    exit(0);
}

int test_check_stack(void) {
    int num_threads = 5;
    int args[5];
    int threads[5];

    global_stack_addresses = malloc(num_threads * sizeof(int*));

    for (int i = 0; i < num_threads; i++) {

        args[i] = i;
        osthread_create(&threads[i], thread_func_check_stack, &args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        osthread_join(threads[i], 0);
    }

    // Check if all threads have different stack addresses
    for (int i = 0; i < num_threads; i++) {
        for (int j = i + 1; j < num_threads; j++) {
            if (global_stack_addresses[i] == global_stack_addresses[j]){
                 printf("Test Check Stack FAILED\n");
                 return 0;
            }
        }
    }

    printf("Test Check Stack PASSED\n");

    free(global_stack_addresses);

    return 0;
}


void* thread_func_sbrk_thread(void* args) { 
    int input = *((int*)args);

    sleep(5 * (input + 1));

    sbrk(PGSIZE);

    exit(0);
}

int test_sbrk_thread(void) {
    int num_threads = 5;

    int args[5];
    int threads[5];

    for (int i = 0; i < num_threads; i++) {
	args[i] = i;
	osthread_create(&threads[i], thread_func_sbrk_thread, &args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
	osthread_join(threads[i], 0);
    }

    printf("Test sbrk Thread PASSED\n");

    return 0;
}

void* thread_func_sbrk(void* args) { 
    int input = *((int*)args);

    sleep(5 * (input + 1));

    exit(0);
}

int test_sbrk(void) {
    int num_threads = 5;

    int args[5];
    int threads[5];


    for (int i = 0; i < num_threads; i++) {
	args[i] = i;
	osthread_create(&threads[i], thread_func_sbrk, &args[i]);
    }

    sbrk(PGSIZE);

    for (int i = 0; i < num_threads; i++) {
	osthread_join(threads[i], 0);
    }

    printf("Test sbrk PASSED\n");

    return 0;
}

void* thread_func_kill_children(void* args) { 
    int input = *((int*)args);

    sleep(10 * (input + 1));

    printf("Thread %d exiting (Test FAILED)\n", input);

    exit(0);
}

int test_kill_children(void) {

    int pid = fork();
    if (pid == 0) {
	int num_threads = 5;

	int args[5];
	int threads[5];


	for (int i = 0; i < num_threads; i++) {
	    args[i] = i;
	    osthread_create(&threads[i], thread_func_kill_children, &args[i]);
	} 

	printf("Test Kill Children PASSED\n");

	exit(0);
    } else {
	wait(0);
    }

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

    for (int i = 0; i < num_threads; i++) {
	args[2 * i] = 2;
	args[2 * i + 1] = i;
	osthread_create(&threads[i], thread_func_write_global_vars, args + 2 * i);
    }

    for (int i = 0; i < num_threads; i++) {
	osthread_join(threads[i], 0);
	printf("Done waiting for thread %d\n", i);
    }

    if (write_global_var != 2 * num_threads) {
	printf("Test Write Global Vars FAILED (write_gloal_var = %d, expected write_global_vars = %d)\n", write_global_var, 2 * num_threads);
    } else {
	printf("Test Write Global Vars PASSED\n");
    }

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

    osthread_create(&thread, thread_func_read_global_vars, &args); 

    osthread_join(thread, 0);

    return 0;
}

void* thread_func_basic_args(void* args) { 
    int* inputs = (int*)args;
    
    for (int i = 0; i < 100; i++) {
      if (inputs[i] != i) {
        printf("Test Basic Args FAILED (expected args[%d] = %d, actual args[%d] = %d)\n", i, i, i, inputs[i]);
	exit(0);
      }
    }

    printf("Test Basic Args PASSED\n");

    exit(0);
}

int test_basic_args(void) {
    osthread thread;

    int args[100];
    for (int i = 0; i < 100; i++) {
      args[i] = i;
    }

    osthread_create(&thread, thread_func_basic_args, &args); 

    osthread_join(thread, 0);

    return 0;
}

void* thread_func_join(void* args) {
    sleep(10);
    printf("Exiting thread for Test Join\n");
    exit(0);
}

int test_join(void) {
    osthread thread;

    osthread_create(&thread, thread_func_join, 0); 

    osthread_join(thread, 0);

    printf("Exiting Test Join\n");

    return 0;
}

int main(int argc, char** argv) {
    test_join();
    test_basic_args();
    test_read_global_vars();
    test_write_global_vars();
    test_kill_children();
    test_sbrk();
    test_sbrk_thread();
    test_check_stack();
    printf("Tests Complete\n");
    exit(0);
}
