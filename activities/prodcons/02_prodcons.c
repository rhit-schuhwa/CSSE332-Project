#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "log.h"

#define NUM_ITERATIONS 10

volatile int burger = 0;

void cook_burger(int id, int i) {
  print_red("Cook thread %d cooking burger %d...\n", id, i);
  assert(!burger);

  burger = 1;
}

void eat_burger(int id, int i) {
  print_green("Eater thread %d eating burger %d...\n", id, i);
  assert(!burger);

  burger = 0;
}

void *cook(void *arg) {
  int i, id;

  // grab the id of the cook thread
  id = *(int*)arg;

  for(i = 0; i < 2*NUM_ITERATIONS; i++) {
    cook_burger(id, i);
  }

  return NULL;
}

void *eater(void *arg) {
  int i, id;

  // grab the id of the eater thread
  id = *(int*)arg;
  for(i = 0; i < NUM_ITERATIONS; i++) {
    eat_burger(id, i);

    // This sleep simulates the cook doing other stuff.
    // Do NOT include this sleep statement in your condition variables/locking
    // mechanisms, it should be floating here on its own.
    sleep(1);
  }

  return NULL;
}

int main(int argc, char **argv) {
  pthread_t producer, consumers[2];
  int i, ids[3] = {1, 2, 3};

  // create the threads
  for(i = 1; i < 3; i++) {
    pthread_create(&consumers[i+1], 0, eater, &ids[i+1]);
  }
  pthread_create(&producer, 0, cook, &ids[0]);

  // join the threads
  pthread_join(producer, 0);
  for(i = 1; i < 3; i++) {
    pthread_join(consumers[i], 0);
  }

  return 0;
}

