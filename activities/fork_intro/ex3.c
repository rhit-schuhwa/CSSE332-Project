#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * Exercise 3
 *
 * In this exercise, we would like to create a cascade of children, each one
 * creating a child of its own.
 *
 * Starting from a parent, we would like to see something like this:
 * parent -> child 1 -> child 2 -> child 3 -> child 4
 *
 * Each process will print its process id and its order in the chain.
 * So parent prints: Parent has pid: <parent pid>
 * First child prints: Child 1 has pid: <pid>
 * Second child prints: Child 2 has pid: <pid>
 *
 * Each parent must wait for its child to exit before they can exit.
 *  This means that the main parent will not exit until ALL children have
 *  exited.
 *
 * OPTIONAL: read the number of children to create from the command line.
 */

// TODO: Add your code here!

void child_gen(int curr_child, int max)
{
    int rc = fork();

    if (rc < 0)
    {
        perror("Failed to fork a process");
        exit(EXIT_FAILURE);
    }

    if (rc == 0)
    {
        printf("Child %d has pid: %d\n", curr_child, getpid());
        if(curr_child < max){
            child_gen(curr_child + 1, max);
        }
        exit(EXIT_SUCCESS);
    }
    else
    {
        wait(NULL);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("USAGE: ./ex3 <num_children>\n");
        exit(EXIT_FAILURE);
    }

    int max = atoi(argv[1]);
    printf("Parent has pid: %d\n", getpid());
    child_gen(1, max);

    exit(EXIT_SUCCESS);
}
