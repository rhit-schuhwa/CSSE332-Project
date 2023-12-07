#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "shell.h"

//
// This one has TWO targets: program.bin and donothing.bin
//
// Task 1:
// =======
//
//  Write down the dependencies between the files.
//
//  Hint: execlp does not count as a build dependecy. Ask yourself why?
//
// Task 2:
// =======
//
//  Write a makefile to build both targets with correct depedencies.
//  When I type make, both executables should be generated, without any
//  unnecessary recompilations.
//

int main(int argc, char **argv) {
  run_shell();

  exit(0);
}
