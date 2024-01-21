#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "log.h"

#define NUM_ITERATIONS 10

#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 5

#define BUF_SIZ 3

/**
 * This is an extension of the producer consumer problem to include 2 producers
 * and 5 consumers that would be accessing a shared circular buffer.
 *
 * First, make sure to read and understand this code, then add your
 * synchronization mechanisms to make sure that the code runs correctly.
 */

// we now have a plate that can hold BUF_SIZ burgers
struct circular_plate {
  int burgers[BUF_SIZ];
  int eat_index;
  int place_index;
  int on_plate;
} plate;

int num_cooked_burgers = 0;


static int is_empty() {
  return plate.on_plate == 0;
}

static int is_full() {
  return plate.on_plate == BUF_SIZ;
}

void cook_burger(int id) {
  int where;

  // make sure plate is not full
  assert(!is_full());

  where = plate.place_index;
  plate.burgers[plate.place_index] = id;
  plate.place_index = (plate.place_index + 1) % BUF_SIZ;
  plate.on_plate++;
  num_cooked_burgers++;

  print_red("Cook thread %d adding burger %d to the plate at %d...\n",
            id, num_cooked_burgers, where);
}

void eat_burger(int id) {
  int where, by;

  // make sure plate is not empty
  assert(!is_empty());

  where = plate.eat_index;
  by = plate.burgers[where];
  plate.eat_index = (plate.eat_index + 1) % BUF_SIZ;
  plate.on_plate--;

  print_green("Eater thread %d ate burger from position %d on plate, made by %d...\n",
              id, where, by);
}

void *cook(void *arg) {
  int i, id;

  // grab the id of the cook thread
  id = *(int*)arg;

  for(i = 0; i < (NUM_CONSUMERS*NUM_ITERATIONS)/NUM_PRODUCERS; i++) {
    cook_burger(id);
  }

  return NULL;
}

void *eater(void *arg) {
  int i, id;

  // grab the id of the eater thread
  id = *(int*)arg;
  for(i = 0; i < NUM_ITERATIONS; i++) {
    eat_burger(id);

    // This sleep simulates the cook doing other stuff.
    // Do NOT include this sleep statement in your condition variables/locking
    // mechanisms, it should be floating here on its own.
    sleep(1);
  }

  return NULL;
}

int main(int argc, char **argv) {
  int i;
  int consumer_ids[NUM_CONSUMERS];
  int producer_ids[NUM_PRODUCERS];

  // intialize the plate
  plate.on_plate = 0;
  plate.eat_index = 0;
  plate.place_index = 0;

  pthread_t producers[NUM_PRODUCERS];
  pthread_t consumers[NUM_CONSUMERS];

  // create the producer threads
  for(i = 0; i < NUM_PRODUCERS; i++) {
    producer_ids[i] = i+1;
    pthread_create(&producers[i], 0, cook, &producer_ids[i]);
  }

  // create the consumer threads
  for(i = 0; i < NUM_CONSUMERS; i++) {
    consumer_ids[i] = i+1;
    pthread_create(&consumers[i], 0, eater, &consumer_ids[i]);
  }

  // join the threads
  for(i = 0; i < NUM_PRODUCERS; i++) {
    pthread_join(producers[i], 0);
  }
  for(i = 0; i < NUM_CONSUMERS; i++) {
    pthread_join(consumers[i], 0);
  }

  return 0;
}

