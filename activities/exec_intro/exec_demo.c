#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int pid = fork();

	if (pid < 0) {
		perror("Fork failed.\n");
		return 1;  // nonzero means a failure result in unix
	}

	if (pid == 0) {
    execlp("./buffalosay.bin", "./buffalosay.bin", "Hi", NULL);

    // When will this line of code ever execute?

    // alternatively:
    // char *args[] = {"./buffalosay.bin", "Hi", NULL};
    // execvp("./buffalosay.bin", args);
	}

  printf("Parent (%d): Waiting for child %d\n", getpid(), pid);
  wait(0);
  printf("Parent (%d): Child is done, goodbye...\n");
	return 0;
}
