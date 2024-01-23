/* Copyright 2021 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

/**
  Imagine a group of friends are getting together to play music, but
  they are arriving at different times.  Arriving can happen at any
  time (e.g. when some other friends are playing).

  There are 3 different kinds of friends - drummers, singers, and
  guitarists.  It takes one of each kind to make a band, plus only
  1 band can be playing at once.  Once those conditions are met, the
  players can start playing and stop playing in any order.  However,
  all 3 players must stop playing before a new set of 3 can start
  playing.

  Example output:

  drummer arrived
  drummer arrived
  guitarist arrived
  guitarist arrived
  singer arrived
  drummer playing
  guitarist playing
  singer playing
  singer arrived
  singer arrived
  drummer arrived
  guitarist arrived
  drummer finished playing
  guitarist finished playing
  singer finished playing
  singer playing
  guitarist playing
  drummer playing
  singer finished playing
  guitarist finished playing
  drummer finished playing
  guitarist playing
  drummer playing
  singer playing
  guitarist finished playing
  drummer finished playing
  singer finished playing
  Everything finished.


 **/

int DRUM = 0;
int SING = 1;
int GUIT = 2;

char* names[] = {"drummer", "singer", "guitarist"};

pthread_cond_t c[] = {PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER};
pthread_cond_t band = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

int num_friends[] = {0, 0, 0};
int full_band = 0;
int is_playing[] = {0, 0, 0};

// because the code is similar, we'll just have one kind of thread
// and we'll pass its kind as a parameter
void* friend(void * kind_ptr) {
  int kind = *((int*) kind_ptr);
  
  printf("%s arrived\n", names[kind]);

  pthread_mutex_lock(&m);
  num_friends[kind]++;
  if (num_friends[0] > 0 && num_friends[1] > 0 && num_friends[2] > 0) {
    full_band = 1;
  }
  while (!full_band) {
    pthread_cond_wait(&c[kind], &m);
  }
  if (full_band) {
    for (int i = 0; i < 3; i++) {
      if (i != kind) {
        pthread_cond_signal(&c[i]);
      }
    }
  }
  while ((kind == 0 && is_playing[0]) || (kind == 1 && is_playing[1]) || (kind == 2 && is_playing[2])) {
    pthread_cond_wait(&band, &m);
  }
  is_playing[kind] = 1;
  pthread_mutex_unlock(&m);

  printf("%s playing\n", names[kind]);
  sleep(1);
  printf("%s finished playing\n", names[kind]);

  pthread_mutex_lock(&m);
  num_friends[kind]--;
  is_playing[kind] = 0;
  if (num_friends[0] == 0 || num_friends[1] == 0 || num_friends[2] == 0) {
    full_band = 0;
  }
  if (!is_playing[0] && !is_playing[1] && !is_playing[2]) {
    pthread_cond_broadcast(&band);
  }
  pthread_mutex_unlock(&m);
  
  return NULL;
}

pthread_t friends[100];
int friend_count = 0;

void create_friend(int* kind) {
  pthread_create(&friends[friend_count], NULL, friend, kind);
  friend_count++;
}

int main(int argc, char **argv) {

  create_friend(&DRUM);
  create_friend(&DRUM);
  create_friend(&GUIT);
  create_friend(&GUIT);
  sleep(1);
  create_friend(&SING);
  create_friend(&SING);
  create_friend(&DRUM);
  create_friend(&GUIT);
  create_friend(&SING);

  // all threads must be created by this point
  // note if you didn't create an equal number of each, we'll be stuck forever
  for (int i = 0; i < friend_count; i++) {
    pthread_join(friends[i], NULL);
  }

  printf("Everything finished.\n");

}
