#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#include <pthread.h>

/**
 * In this exercise, we are trying to sum up an array of numbers using
 * pthreads. You must use appropriate locking mechanisms to ensure correct
 * outcomes of the array, and must do so efficiently.
 */

int *array;
int array_size;
int share;
volatile unsigned long long sum;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static void output_time_difference(char* name, struct timeval* start,
                                   struct timeval* end) {
  long secs_used =
    (end->tv_sec - start->tv_sec);  // avoid overflow by subtracting first
  long usecs_used = (end->tv_usec - start->tv_usec);
  double secs = secs_used + (double)usecs_used / 1000000;
  printf("%s took %f seconds\n", name, secs);
}

void *run_fn(void *arg) {
  // TODO: Add your thread code here...
  int idx = *(int*)arg;
  unsigned long long local_sum = 0;
  for (int i = idx; i < idx + share; i++) {
    local_sum += array[i];
  }
  pthread_mutex_lock(&lock);
  sum += local_sum;
  pthread_mutex_unlock(&lock);
  return NULL;
}

int main(int argc, char **argv) {
  struct timeval start, end;

  if (argc > 1)
    array_size = atoi(argv[1]);
  else
    array_size = 1024;

  if (array_size % 2) {
    fprintf(stderr, "Please enter a multiple of two for the size\n");
    exit(EXIT_FAILURE);
  }

  /* initialize the array */
  array = malloc(array_size * sizeof(int));
  for (int i = 0; i < array_size; ++i) {
    array[i] = 1;
  }

  /* first do it sequentially */
  unsigned long long seq_sum = 0;
  gettimeofday(&start, NULL);
  for (int i = 0; i < array_size; i++) {
    seq_sum += array[i];
  }
  gettimeofday(&end, NULL);
  printf("== \tThe value of the sum under sequential execution is %llu ==\n", seq_sum);
  output_time_difference("== \tSequential sum", &start, &end);

  // TODO:
  // =====
  //  Now do the sum using two threads.
  //

  // TODO: Add any initialization code here.
  pthread_t p1, p2;
  share = array_size/2;
  int id1 = 0, id2 = share;

  gettimeofday(&start, NULL);
  // TODO: Add your pthread creation and joining code here bet the first call
  // to gettimeofday and the second call to gettimeofday.
  pthread_create(&p1, NULL, run_fn, &id1);
  pthread_create(&p2, NULL, run_fn, &id2);

  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  
  gettimeofday(&end, NULL);

  printf("== \tThe value of the sum under threads is %llu ==\n", sum);
  output_time_difference("== \tThreaded sum", &start, &end);

  exit(EXIT_SUCCESS);
}
