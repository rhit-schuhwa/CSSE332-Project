#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * pipe_pid.c
 *
 * Task:
 * =====
 * In this exercise, we would like to child to send its process id back to its
 * parent. Yes we can get it from the return value of fork, but we'd like to
 * have a backup way for getting it as well.
 *
 * HINT 1:
 * =======
 * You might find the functions sprintf, snprintf, and atoi very helpful.
 *
 * HINT 2:
 * =======
 *  Alternatively, remember that everything is just a bunch of bytes.  How many
 *  bytes is an integer?
 */

int main(int argc, char **argv) {
  int fd[2];
  pid_t pid;
  char readbuff[5];

  if (pipe(fd) < 0) {
    perror("PANIC: pipe failed: ");
    exit(99);
  }

  pid = fork();
  if(pid == 0) {
    // child, I am the writer
    close(fd[0]);
    printf("Child pid: %d\n", getpid());
    int cpid = getpid();
    write(fd[1], &cpid, sizeof(int));
    close(fd[1]);
    exit(0);
  } else {
    // parent, I am the reader
    close(fd[1]);
    read(fd[0], readbuff, sizeof(int));
    readbuff[sizeof(int)] = 0;
    int cpid = *((int*)readbuff);
    printf("Parent read from child: %d\n",  cpid);
  }
  close(fd[0]);
  exit(0);
}

