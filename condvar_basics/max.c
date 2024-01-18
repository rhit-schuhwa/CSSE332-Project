#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_cond_t c = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int curr = 0;

void *thread(void *arg)
{
	char *letter = (char *)arg;

	printf("%c wants to enter the critical section\n", *letter);
	
	pthread_mutex_lock(&m);
	while (curr >= 3) {
	    pthread_cond_wait(&c, &m);
	}
	curr++;
	pthread_mutex_unlock(&m);

	printf("%c is in the critical section\n", *letter);
	sleep(1);
	printf("%c has left the critical section\n", *letter);

	pthread_mutex_lock(&m);
	curr--;
	pthread_cond_signal(&c);
	pthread_mutex_unlock(&m);

	return NULL;
}

int
main(int argc, char **argv)
{
	pthread_t threads[8];
	int i;
	char *letters = "abcdefgh";

	for(i = 0; i < 8; ++i) {
		pthread_create(&threads[i], NULL, thread, &letters[i]);

		if(i == 4)
			sleep(4);
	}

	for(i = 0; i < 8; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("Everything finished...\n");

	return 0;
}
