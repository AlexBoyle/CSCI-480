//includes 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

// Constants
#define P_NUMBER        6
#define C_NUMBER        4
#define BUFFER_SIZE    12
#define P_STEPS       4
#define C_STEPS       6

//Globals
pthread_mutex_t lock;
sem_t NotFull ; 
sem_t NotEmpty; 
int widgets = 0;

/**
*Insert a new widget
*params threadid: id of the calling thread
*/
void Insert(void * threadid)
{
	long tid = (long) threadid;
	pthread_mutex_lock(&lock); // Begining of cpu lock
	widgets ++;
	printf("Producer %ld inserted one item. Total is now %i\n", tid, widgets);
	pthread_mutex_unlock(&lock);// End of cpu lock
}

/**
*Body of the producer thread
*params threadid: id of the thread
*/
void * Produce(void * threadid)
{
	for(int i = 0; i < P_STEPS; i ++) {
		sem_wait(&NotFull);// if fill wait
		Insert(threadid);// Insert
		sem_post(&NotEmpty);// if say that list is not empty
		sleep(1);// sleep 
	}
	pthread_exit(NULL);// exit thread
}

/**
*Removes a widget
*params threadid: id of the calling thread
*/
void Remove(void * threadid)
{
	long tid = (long) threadid;
	pthread_mutex_lock(&lock);// begining of cpu lock
	widgets --;
	printf("Consumer %ld removed one item.  Total is now %i\n", tid, widgets);
	pthread_mutex_unlock(&lock);// End of cpu lock
}

/**
*Body of the consumer thread
*params threadid: id of the thread
*/
void * Consume(void * threadid)
{
		for(int i = 0; i < C_STEPS; i ++) {
		sem_wait(&NotEmpty);// if empty wait
		Remove(threadid);// Remove 
		sem_post(&NotFull);// signle that the list is not full
		sleep(1);
	}
	pthread_exit(NULL);// End thread
}

int main (int argc, char *argv[])
{
	sem_init(&NotFull, 0, BUFFER_SIZE);// initalize NotFull
	sem_init(&NotEmpty, 0, 0);// initalize NotEmpty
	if (pthread_mutex_init(&lock, NULL) != 0) { // initalize lock and check if it failed
		printf("\n mutex init failed\n");
		return 1;
	}
	int total_threads = P_NUMBER + C_NUMBER;
	pthread_t threads[total_threads];// array of threads
	int rc;// temp var
	for(long i = 0; i < P_NUMBER; i++) {
		rc = pthread_create(&threads[i], NULL, Produce, (void *) i);// start new producer thread
		if (rc) {exit(-1);}//exit if fail
	}
	for(long i = P_NUMBER; i < total_threads; i++) {
		rc = pthread_create(&threads[i], NULL, Consume, (void *) (i - P_NUMBER));// start new consumer thread
		if (rc) {exit(-1);}//exit if fail
	}
	char *b;
	for(int i = 0; i < total_threads; i ++) {
		pthread_join(threads[i], (void**)&b);// wait for threads to finish
	}
	printf("\n\nAll the producer and consumer threads have been closed.\n");
	pthread_mutex_destroy(&lock);// destroy mutex
	sem_destroy(&NotFull);// destroy semaphore
	sem_destroy(&NotEmpty);// destroy semaphore
}