#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "shell.h"

void run_shell() {
  if(fork() == 0) {
    execlp("./donothing.bin", "./donothing.bin", NULL);
    perror("exec failed");
    exit(1);
  }

  wait(0);
  printf("Goodbye!\n");
  exit(0);
}
