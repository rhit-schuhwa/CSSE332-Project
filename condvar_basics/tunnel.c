#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

// constants to control number of cars and threads
#define NUM_EW_CARS 15
#define NUM_WE_CARS 15
#define NUM_AMBULANCE 7
#define TOTAL_THREADS NUM_EW_CARS + NUM_WE_CARS + NUM_AMBULANCE

// constants to define ambulance direction
#define EW_DIRECTION 1
#define WE_DIRECTION 2

// car speeds
#define EW_SPEED 3
#define WE_SPEED 2

// state variables
int num_ew_car = 0;
int num_we_car = 0;
int num_amb = 0;
int num_ew_amb = 0;
int num_we_amb = 0;

// concurrency means
pthread_cond_t c = PTHREAD_COND_INITIALIZER;
pthread_cond_t amb = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void *ewcar(void *arg)
{
  int tid = *(int*)arg;

  pthread_mutex_lock(&m);
  while (num_ew_car >= 3) {
    pthread_cond_wait(&c, &m);
  }
  num_ew_car++;

  while (num_amb > 0) {
    pthread_cond_wait(&amb, &m);
  }
  pthread_mutex_unlock(&m);

  printf("Car (%d) entered tunnel in EW direction...\n", tid);
  sleep(EW_SPEED);
  printf("Car (%d) exited tunnel in EW direction...\n", tid);

  pthread_mutex_lock(&m);
  num_ew_car--;
  pthread_cond_broadcast(&c);
  pthread_mutex_unlock(&m);

  return 0;
}

void *wecar(void *arg)
{
  int tid = *(int *)arg;

  pthread_mutex_lock(&m);
  while (num_we_car >= 1) {
    pthread_cond_wait(&c, &m);
  }
  num_we_car++;

  while (num_amb > 0) {
    pthread_cond_wait(&amb, &m);
  }
  pthread_mutex_unlock(&m);
  
  printf("Car (%d) entered tunnel in WE direction...\n", tid);
  sleep(WE_SPEED);
  printf("Car (%d) exited tunnel in WE direction...\n", tid);

  pthread_mutex_lock(&m);
  num_we_car--;
  pthread_cond_broadcast(&c);
  pthread_mutex_unlock(&m);
  
  return 0;
}

void *ambulance(void *arg)
{
  int tid = *(int*)arg;
  int direction = EW_DIRECTION;
  int is_ew = 0;

  pthread_mutex_lock(&m);
  while ((num_ew_car >= 3 && num_we_car >= 1) || (num_ew_car + num_we_car + num_amb) >= 4) {
    pthread_cond_wait(&c, &m);
  }
  while (num_amb >= 4) {
    pthread_cond_wait(&amb, &m);
  }
  num_amb++;

  if (num_ew_car >= 3 && num_we_amb == 0) {
    num_we_amb++;
    direction = WE_DIRECTION;
  } else {
    is_ew = 1;
    num_ew_amb++;
  }

  if(direction == EW_DIRECTION) {
    printf("Ambulance %d entered the tunnel in EW direction\n", tid);
  } else if (direction == WE_DIRECTION) {
    printf("Ambulance %d entered the tunnel in WE direction\n", tid);
  }
  pthread_mutex_unlock(&m);

  sleep(1); // ambulance is the fastest

  pthread_mutex_lock(&m);
  if(direction == EW_DIRECTION) {
    printf("Ambulance %d exited the tunnel in EW direction\n", tid);
  } else if(direction == WE_DIRECTION) {
    printf("Ambulance %d exited the tunnel in WE direction\n", tid);
  }

  if (is_ew) {
    num_ew_amb--;
  } else {
    num_we_amb--;
  }
  
  num_amb--;
  pthread_cond_broadcast(&c);
  pthread_cond_broadcast(&amb);
  pthread_mutex_unlock(&m);

  return 0;
}


int
main(int argc, char **argv)
{
  pthread_t threads[NUM_EW_CARS + NUM_WE_CARS + NUM_AMBULANCE];
  int tids[NUM_EW_CARS + NUM_WE_CARS + NUM_AMBULANCE];
  int i;
  srand(time(0));

  for(i = 0; i < NUM_EW_CARS; i++) {
    tids[i] = i + 1;
    pthread_create(&threads[i], 0, ewcar, &tids[i]);
  }

  for(i = NUM_EW_CARS; i < NUM_WE_CARS + NUM_EW_CARS; i++) {
    tids[i] = i + 1;
    pthread_create(&threads[i], 0, wecar, &tids[i]);
  }

  for(i = NUM_EW_CARS + NUM_WE_CARS; i < TOTAL_THREADS; i++) {
    tids[i] = i + 1;
    pthread_create(&threads[i], 0, ambulance, &tids[i]);
    // make the ambulances arrive at random times
    sleep(rand() % 10);
  }

  for(i = 0; i < TOTAL_THREADS; i++) {
    pthread_join(threads[i], 0);
  }

  printf("Everyting finished...\n");
  exit(EXIT_SUCCESS);
}
