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

void* thread_func(void* args) { 
   int* inputs = (int*)args;
   printf("%d + %d = %d\n", inputs[0], inputs[1], inputs[0] + inputs[1]);

   exit(0);
}

int testBasicArgs(void) {
    osthread thread;

    int args[] = {14, 43};
    char* stack = malloc(PGSIZE);

    osthread_create(&thread, thread_func, &args, stack); 

    sleep(5);

    free(stack);
    exit(0);
}

int main(int argc, char** argv) {
    testBasicArgs();
    exit(0);
}
