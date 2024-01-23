#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


/**
  In this system there are threads numbered 1-6 and a critical
  section.  The thread numbers are priorities, where thread 6 is
  highest priority, thread 5 is next, etc.

  If the critical section is empty, any thread can enter.  While a
  thread is in the critical section no other threads can enter -
  regardless of priority. When a thread leaves the critical section
  and there are threads waiting, the highest priority waiting thread
  is allowed to enter the critical section.

  Be sure a newly arriving thread can't jump into the critical
  section as the current thread finishes, bypassing priority rules.
  Solve this problem with mutexes/condition variables
 **/

pthread_cond_t c = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

int priority = 0;
int active = 0;
int nums[6];
int i = 0;
int num_waiting = 0;

void *thread(void *arg)
{
  int *num = (int *) arg;
  printf("%d wants to enter the critical section\n", *num);

  pthread_mutex_lock(&m);
  if (*num > priority) {
    priority = *num;
  }
  nums[i] = *num;
  i++;
  while (active || *num != priority) {
    pthread_cond_wait(&c, &m);
  }
  active = 1;
  pthread_mutex_unlock(&m);

  printf("%d has entered the critical section\n", *num);
  sleep(1);
  printf("%d is finished with the critical section\n", *num);

  pthread_mutex_lock(&m);
  active = 0;
  priority = nums[0];
  for (int j = 0; j < 6; j++) {
    if (nums[j] == *num) {
      nums[j] = 0;
    }
    if (nums[j] > priority) {
      priority = nums[j];
    }
  }
  pthread_cond_broadcast(&c);
  pthread_mutex_unlock(&m);

  return NULL;
}

int
main(int argc, char **argv)
{
  int i;
  pthread_t threads[6];
  int nums[] = {2, 1, 4, 3, 5, 6};


  for(i = 0; i < 6; i++) {
    pthread_create(&threads[i], NULL, thread, &nums[i]);

    if(i == 2) sleep(10);
  }

  for(i = 0; i < 6; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Everything finished.\n");
}
