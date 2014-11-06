#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

#define SLEEP 10000

/* TODO */
/*
 * Fix how to cancel threads.
 * Fix condition var to end reader loop
 * Looping one too many
 * Wew
 */

static void * reader(void *in);
static void * calculator(void *in);
static void * control(void *in);

//struct to hold data
typedef struct {
	int x;
	int y;
	FILE *fp;
	pthread_t r;
	pthread_t c;
	int randomSleep;
} values_t;

static void cancelThreads(int signo) {
	printf("Goodbye from Thread 2");
}

static void getRandomSleep(int* x) {
	srand(time(NULL));
	*x = rand_r() % SLEEP;
}//generates a random sleep time

static void randomuSleep(int x) {
	getRandomSleep(&x);
	usleep(x);
}//generates a random uSleep

static void * reader(void *val_in) {
//	sleep(1); //hack to fix seg fault????
	values_t *val = val_in;
	val->r = pthread_self();
	sigset_t set;
	int sig;

	//set signal
	sigemptyset(&set);
	sigaddset(&set,SIGUSR2);
	sigset(SIGUSR1, calculator);

	while(!feof(val->fp)) {
		//read in the two ints
		fscanf(val->fp, "%d %d", &val->x, &val->y);

		//print out values to be calculated
		printf("Thread 1 submitting : %d %d\n", val->x, val->y);

		randomuSleep(val->randomSleep);

		//signal for signal
		pthread_kill(val->c,SIGUSR1);

		//wait
		sigwait(&set, &sig);

		//sleep
		//randomuSleep(val->randomSleep);
		//sleep(1);
	}

	//sigwait(&set, &sig);

	//printf("Goodbye from Thread 1");

	//pthread_kill(val->c,SIGUSR1);

	return((void *)NULL);
}//reader

static void * calculator(void *val_in) {
	values_t *val = val_in;
	val->c = pthread_self();
	sigset_t set;
	int sig;

	//set signal
	sigemptyset(&set);
	sigset(SIGUSR2, reader);
	//sigset(SIGINT,cancelThreads);
	sigaddset(&set, SIGUSR1);

	while(!feof(val->fp)) {
		//randomuSleep(val->randomSleep);
		//wait for signal
		sigwait(&set, &sig);

		//calculate 
		printf("Thread 2 calculated : %d\n", val->x + val->y);

		//signal
		randomuSleep(val->randomSleep);
		pthread_kill(val->r,SIGUSR2);
		
		//sleep
		//randomuSleep(val->randomSleep);
		//sleep(1);
	}

	//printf("Goodbye from Thread 2");

	return((void *)NULL);
}//calculator

int main(int arc, char *argv[]) {
	values_t val;
	sigset_t set;

	sigemptyset(&set);
	//sigset(SIGINT,cancelThread);

	//open file
	val.fp = fopen(argv[1], "r");

	if(val.fp == NULL) {
		perror("error opening file");
		return(0);
	}//ensure file valid

	//init threads
	pthread_t read_t;
	pthread_t calc_t;

	//create threads
	pthread_create(&read_t, NULL, &reader, (void *)&val);
	pthread_create(&calc_t, NULL, &calculator, (void *)&val);

	//start threads
	pthread_join(read_t, NULL);
	pthread_join(calc_t, NULL);

	//close file
	fclose(val.fp);
	return(0);
}//main
