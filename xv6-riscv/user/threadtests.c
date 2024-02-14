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

int read_global_var = 15;

void* thread_func_read_global_vars(void* args) { 
    int input = *((int*)args);
    if (input * read_global_var != 15 * 17) {
	printf("Test Read Global Vars FAILED (input * read_global_vars = %d, expected input * read_global_vars = %d)\n", input * read_global_var, 15 * 17);
    } else {
	printf("Test Read Global Vars PASSED\n");
    }
    return 0;
}

int test_read_global_vars(void) {
    osthread thread;

    int args[] = {17};
    char* stack = malloc(PGSIZE);

    osthread_create(&thread, thread_func_read_global_vars, &args, stack); 
printf("zzzz\n");
    sleep(5);

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
    return 0;
}

int test_basic_args(void) {
    osthread thread;

    int args[] = {14, 43};
    char* stack = malloc(PGSIZE);

    osthread_create(&thread, thread_func_basic_args, &args, stack); 

    sleep(5);

    free(stack);
    return 0;
}

int main(int argc, char** argv) {
    test_basic_args();
    test_read_global_vars();
    exit(0);
}
