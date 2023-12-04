#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int pid = fork();
  int status;
  int rc;

	if (pid < 0) {
		perror("Fork failed.\n");
		return 1;  // nonzero means a failure result in unix
	}

	if (pid == 0) {
    printf("I am the child and my PID is %d\n", getpid());
    sleep(1);
    printf("Child (%d): Done and will exit with value 3!\n", getpid());

    exit(3); // alternatively, can do return 3;
	}

  printf("Parent (%d): I created a child with PID: %d\n", getpid(), pid);
  rc = wait(&status);
  if(rc < 0) {
    fprintf(stderr, "Parent (%d): Something bad happened to my child!\n",
        getpid());
    exit(EXIT_FAILURE);
  }

  printf("Parent (%d): Child %d done\n", getpid(), rc);
  if(WIFEXITED(status)) {
    // child exited
    printf("Parent (%d): Child (%d) returned with exit code %d\n",
        getpid(), pid, WEXITSTATUS(status));
  }

	return 0;
}
