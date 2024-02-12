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

void thread_func(void) {
   printf("thread\n");

   exit(0);
}

int main(int argc, char** argv) {
    printf("thread_func: %p\n", thread_func);

    osthread thread;

    osthread_create(&thread, thread_func, (void*)20);

    printf("thread num: %d\n", thread);

    exit(0);
}
