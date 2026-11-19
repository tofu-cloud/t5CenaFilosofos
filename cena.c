#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int state[N];
sem_t mutex;
sem_t s[N];
pthread_t philosophers[N];

int LEFT(int i)
{
	return (i + N - 1) % N;
}
int RIGHT(int i)
{
	return (i + 1) % N;
}

void test(int i)
{
	if(state[i] == HUNGRY &&
   	state[LEFT(i)] != EATING &&
   	state[RIGHT(i)] != EATING)
	{
    	state[i] = EATING;
    	printf("Filósofo %d toma tenedores %d y %d\n",
           	i, LEFT(i), i);
    	printf("Filósofo %d está COMIENDO\n", i);
    	sem_post(&s[i]);
	}
}

void take_forks(int i)
{
	sem_wait(&mutex);
	state[i] = HUNGRY;
	printf("Filósofo %d tiene hambre\n", i);
	test(i);
	sem_post(&mutex);
	sem_wait(&s[i]);
}

void put_forks(int i)
{
	sem_wait(&mutex);
	state[i] = THINKING;
	printf("Filósofo %d deja los tenedores\n", i);
	printf("Filósofo %d está PENSANDO\n", i);
	test(LEFT(i));
	test(RIGHT(i));
	sem_post(&mutex);
}

void* philosopher(void *num)
{
	int i = *(int*)num;
	for(int k = 0; k < 5; k++)
	{
    	sleep(1);
    	printf("Filósofo %d está pensando\n", i);
    	sleep(1);
    	take_forks(i);
    	sleep(2);
    	put_forks(i);
	}
	return NULL;
}

int main()
{
	int ids[N];
	sem_init(&mutex, 0, 1);
	for(int i = 0; i < N; i++)
	{
    	sem_init(&s[i], 0, 0);
	}
	for(int i = 0; i < N; i++)
	{
    	ids[i] = i;
    	pthread_create(
        	&philosophers[i],
        	NULL,
        	philosopher,
        	&ids[i]
    	);
	}
	for(int i = 0; i < N; i++)
	{
    	pthread_join(philosophers[i], NULL);
	}
	sem_destroy(&mutex);
	for(int i = 0; i < N; i++)
	{
    	sem_destroy(&s[i]);
	}
	return 0;
}
