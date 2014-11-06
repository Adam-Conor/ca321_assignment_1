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
 * Figure out how to signal other threads
 * Wew
 * Get rand to assign in there
 */

static void * reader(void *in);
static void * calculator(void *in);
static void * control(void *in);

//struct to hold data
typedef struct {
	int x;
	int y;
	FILE *fp;
	int randomSleep;
} values_t;

static void cancelThread(int signo) {
	printf("Goodbye from Thread: %d\n", signo);
}

static void getRandom(int* x) {
	srand(time(NULL));
	*x = rand_r() % SLEEP;
}

static void randomuSleep(int x) {
	getRandom(&x);
	usleep(x);
}

static void * reader(void *val_in) {
	values_t *val = val_in;
	sigset_t set;
	int sig;

	//set signal
	sigemptyset(&set);
	sigaddset(&set,SIGUSR1);
	sigaddset(&set,SIGUSR2);
	//sigset(SIGINT,cancelThread);
	sigset(SIGUSR1, calculator);
	//sigsuspend(&set);

	while(!feof(val->fp)) {
		//read in the two ints
		fscanf(val->fp, "%d %d", &val->x, &val->y);

		//print out values to be calculated
		printf("Thread 1 submitting : %d %d\n", val->x, val->y);

		kill(getpid(),SIGUSR1);

		//getRandom(&val->randomSleep);
		//usleep(val->randomSleep);
		randomuSleep(val->randomSleep);

		sigwait(&set, &sig);
		//sigsuspend(&set);
		//sleep
		//usleep(SLEEP);
	}

	//kill(pthread_self(),SIGINT);

	return((void *)NULL);
}//reader

static void * calculator(void *val_in) {
	values_t *val = val_in;
	sigset_t set;
	int sig;

	//set signal
	sigemptyset(&set);
	sigset(SIGUSR2, reader);
	sigaddset(&set, SIGUSR1);

	while(1) {
		//wait for signal
		sigwait(&set, &sig);

		//calculate 
		printf("Thread 2 calculated : %d\n", val->x + val->y);

		kill(getpid(),SIGUSR2);
		
		//usleep(SLEEP);
		randomuSleep(val->randomSleep);
	}

	return((void *)NULL);
}//calculator

static void * control(void *in) {
	char** argv = in;
	values_t val;
	sigset_t set;

	sigemptyset(&set);
	sigset(SIGINT,cancelThread);

	//open file
	val.fp = fopen(argv[1], "r");

	if(val.fp == NULL) {
		perror("error opening file");
		return((void *)NULL);
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
	return((void *)NULL);
}//control

int main(int arc, char *argv[]) {
	pthread_t cont_t; //init thread

	pthread_create(&cont_t, NULL, &control, (void *)argv); //create thread

	pthread_join(cont_t, NULL); //start thread

	return(0);
}
