/* Copyright 2019 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


// number of carpenters
#define NUM_CARP 3
// number of painters
#define NUM_PAIN 3
// number of decorators
#define NUM_DECO 3

/**
  Imagine there is a shared memory space called house.

  There are 3 different kinds of operations on house: carpenters, painters, and
  decorators.  For any particular kind of operation, there can be an unlimited
  number of threads doing the same operation at once (e.g. unlimited carpenter
  threads etc.).  However, only one kind of operation can be done at a time (so
  even a single carpenter should block all painters and vice versa).

  Use mutex locks and condition variables to enforce this constraint.  You don't
  have to worry about starvation (e.g. that constantly arriving decorators might
  prevent carpenters from ever running) - though maybe it would be fun to
  consider how you would solve in that case.

  This is similar to the readers/writers problem BTW.
 **/

pthread_cond_t c = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

int num_jobs[] = {0, 0, 0};

void* carpenter(void * ignored) {

  pthread_mutex_lock(&m);
  while (num_jobs[1] > 0 || num_jobs[2] > 0) {
    pthread_cond_wait(&c, &m);
  }
  num_jobs[0]++;
  pthread_mutex_unlock(&m);

  printf("starting carpentry\n");
  sleep(1);
  printf("finished carpentry\n");

  pthread_mutex_lock(&m);
  num_jobs[0]--;
  if (num_jobs[0] == 0) {
    pthread_cond_broadcast(&c);
  }
  pthread_mutex_unlock(&m);

  return NULL;
}

void* painter(void * ignored) {

  pthread_mutex_lock(&m);
  while (num_jobs[0] > 0 || num_jobs[2] > 0) {
    pthread_cond_wait(&c, &m);
  }
  num_jobs[1]++;
  pthread_mutex_unlock(&m);

  printf("starting painting\n");
  sleep(1);
  printf("finished painting\n");
  
  pthread_mutex_lock(&m);
  num_jobs[1]--;
  if (num_jobs[1] == 0) {
    pthread_cond_broadcast(&c);
  }
  pthread_mutex_unlock(&m);

  return NULL;
}

void* decorator(void * ignored) {

  pthread_mutex_lock(&m);
  while (num_jobs[0] > 0 || num_jobs[1] > 0) {
    pthread_cond_wait(&c, &m);
  }
  num_jobs[2]++;
  pthread_mutex_unlock(&m);
  
  printf("starting decorating\n");
  sleep(1);
  printf("finished decorating\n");
  
  pthread_mutex_lock(&m);
  num_jobs[2]--;
  if (num_jobs[2] == 0) {
    pthread_cond_broadcast(&c);
  }
  pthread_mutex_unlock(&m);

  return NULL;
}


int main(int argc, char **argv) {
  pthread_t jobs[NUM_CARP + NUM_PAIN + NUM_DECO];
  for (int i = 0; i < NUM_CARP + NUM_PAIN + NUM_DECO; i++) {
    void* (*func) (void*) = NULL;
    if(i < NUM_CARP)
      func = carpenter;
    if(i >= NUM_CARP && i < NUM_CARP + NUM_PAIN)
      func = painter;
    if(i >= NUM_CARP + NUM_PAIN) {
      func = decorator;
    }
    pthread_create(&jobs[i], NULL, func, NULL);
  }

  for (int i = 0; i < NUM_CARP + NUM_PAIN + NUM_DECO; i++) {
    pthread_join(jobs[i], NULL);
  }

  printf("Everything finished.\n");

}
