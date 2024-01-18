#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

// Constants in the code
#define FIRST_ROOM_CAPACITY 5
#define FIRST_ROOM_SERVICE_RATE 1

#define SECOND_ROOM_CAPACITY 2
#define SECOND_ROOM_SERVICE_RATE 2
#define WAITING_ROOM_CAPACITY 2

#define NUM_PEOPLE_ARRIVING 10

// capture the of customers that leave early
int num_left_customers = 0;

// TODO: Define other state of the world variables here
int num_first_room = 0;
int num_second_room = 0;
int num_wait = 0;

// TODO: Define your mutex locks and condition variables:
pthread_cond_t c1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t c2 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

void *customer(void *arg)
{
  long int tid = (long int)arg;

  printf("[Customer %ld] Just arrived at first room...\n", tid);

  pthread_mutex_lock(&m1);
  while (num_first_room >= FIRST_ROOM_CAPACITY) {
    pthread_cond_wait(&c1, &m1);
  }
  num_first_room++;
  pthread_mutex_unlock(&m1);

  // Enter the first room.
  printf("[Customer %ld] Entered first room...\n", tid);
  sleep(FIRST_ROOM_SERVICE_RATE);
  printf("[Customer %ld] Left first room...\n", tid);

  pthread_mutex_lock(&m1);
  num_first_room--;
  pthread_cond_signal(&c1);
  pthread_mutex_unlock(&m1);

  // You might want to check if you need to enter the waiting room here or leave
  // here...
  // if(some condition) {
  //   printf("[Customer %ld] Left unhapy because waiting room is full...\n", tid);
  //   return 0;
  // }

  if (num_wait >= WAITING_ROOM_CAPACITY) {
    printf("[Customer %ld] Left unhapy because waiting room is full...\n", tid);
    pthread_mutex_lock(&m2);
    num_left_customers++;
    pthread_mutex_unlock(&m2);
    return 0;
  }

  // In case the customer wants to enter the waiting room, you might want to
  // print the following:
  printf("[Customer %ld] Joined the waiting room for second room...\n", tid);
  
  pthread_mutex_lock(&m2);
  num_wait++;
  while (num_second_room >= SECOND_ROOM_CAPACITY) {
    pthread_cond_wait(&c2, &m2);
  }
  num_second_room++;
  num_wait--;
  pthread_mutex_unlock(&m2);
  
  // Enter the second room
  printf("[Customer %ld] Entered second room...\n", tid);
  sleep(SECOND_ROOM_SERVICE_RATE);
  printf("[Customer %ld] Left second room...\n", tid);

  pthread_mutex_lock(&m2);
  num_second_room--;
  pthread_cond_signal(&c2);
  pthread_mutex_unlock(&m2);
  
  // Done, time to leave...
  return 0;
}

int
main(int argc, char **argv)
{
  int i;
  long int tids[NUM_PEOPLE_ARRIVING];
  pthread_t threads[NUM_PEOPLE_ARRIVING];
  srand(time(0));

  for(i = 0; i < NUM_PEOPLE_ARRIVING; i++) {
    tids[i] = i + 1;
    pthread_create(&threads[i], 0, customer, (void*)tids[i]);
    if(!(i % 2))
      sleep(rand() % 2);
  }

  for(i = 0; i < NUM_PEOPLE_ARRIVING; i++) {
    pthread_join(threads[i], 0);
  }

  printf("Everything finished: %d customers left unhappy...\n",
         num_left_customers);
  exit(EXIT_SUCCESS);
}
