#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// initialized global variable
static int global_x = 3;
// uninitialized global variable
static int global_y;

int main(int argc, char **argv)
{
  pid_t mypid = getpid();
  int i;
  // allocate one byte on the heap
  char *ptr = malloc(1);

  printf("My process id is: %d\n", mypid);
  printf("You are running the program: %s\n", argv[0]);

  if(argc > 1) {
    printf("You have passed %d arguments\n", argc-1);
    for(i = 1; i < argc; i++) {
      printf("\t Argument %d is %s\n", i, argv[i]);
    }
  }

  printf("+------------------------------------------+\n");
  printf("Address of a piece of code: \t%p\n", &main);
  printf("Address of global_x:        \t%p\n", &global_x);
  printf("Address of global_y:        \t%p\n", &global_y);
  printf("Address of (local) i:       \t%p\n", &i);
  printf("Address of (dynamic) ptr:   \t%p\n", ptr);
  printf("+------------------------------------------+\n");

  // going into an infinite loop
  printf("I will go into an infinite loop now, press C-c to exit!\n");
  while(1);

  exit(EXIT_SUCCESS);
}
