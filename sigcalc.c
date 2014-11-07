/*
 * Conor Smyth 12452382
 * Adam O'Flynn 12378651
 * All work is our own
 */

#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

#define SLEEP 10000

static void * reader(void *in);
static void * calculator(void *in);

//struct to hold global data
typedef struct {
	int x;
	int y;
	FILE *fp;
	pthread_t cont;
	int randomSleep;
} values_t;

static void getRandomSleep(int* x) {
	srand(time(NULL));
	*x = rand_r() % SLEEP;
}//generates a random sleep time

static void randomuSleep(int x) {
	getRandomSleep(&x);
	usleep(x);
}//generates a random uSleep

static void * reader(void *val_in) {
	values_t *val = val_in;
	sigset_t set;
	int sig;

	//set signal
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	sigprocmask(SIG_BLOCK, &set, NULL);

	while(sig != SIGINT) {
		//read in the two ints
		fscanf(val->fp, "%d %d", &val->x, &val->y);

		//print out values to be calculated
		printf("Thread 1 submitting : %d %d\n", val->x, val->y);

		//signal for signal
		pthread_kill(val->cont, SIGUSR1);

		//wait
		sigwait(&set, &sig);

		//sleep
		randomuSleep(val->randomSleep);
	}//loop until last integer read

	//Goodbye message
	printf("Goodbye from reader thread\n");

	//signal that finished
	pthread_kill(val->cont, SIGUSR1);

	return((void *)NULL);
}//reader

static void * calculator(void *val_in) {
	values_t *val = val_in;
	sigset_t set;
	int sig;

	//set signals
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	sigprocmask(SIG_BLOCK, &set, NULL);

	//avoid thread going before ready
	sigwait(&set, &sig);

	int calculation = 0;

	while(sig != SIGINT) {
		//calculate 
		calculation = val->x + val->y;
		printf("Thread 2 calculated : %d\n", calculation);

		//signal
		pthread_kill(val->cont, SIGUSR2);

		//wait for numbers
		sigwait(&set, &sig);

		//sleep
		randomuSleep(val->randomSleep);
	}//cancel when SIGINT received

	//Goodbye message
	printf("Goodbye from calculator thread\n");

	return((void *)NULL);
}//calculator

int main(int arc, char *argv[]) {
	values_t val;
	sigset_t set;
	int sig;

	//set signals
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	sigprocmask(SIG_BLOCK, &set, NULL);

	//set main thread ID for child threads
	val.cont = pthread_self();

	//open file
	val.fp = fopen(argv[1], "r");

	if(val.fp == NULL || arc != 2) {
		printf("usage: ./sigcalc file\n");
		return(0);
	}//ensure file valid

	//init threads
	pthread_t read_t;
	pthread_t calc_t;

	//create threads
	pthread_create(&read_t, NULL, &reader, (void *)&val);
	pthread_create(&calc_t, NULL, &calculator, (void *)&val);

	while(!feof(val.fp)) {
		//wait for reader to read integer
		sigwait(&set, &sig);

		//signal integer read
		pthread_kill(calc_t, SIGUSR1);

		//wait for calculation to complete
		sigwait(&set, &sig);

		//signal to read another integer
		pthread_kill(read_t, SIGUSR2);
	}//continue until end of file

	//cancel threads
	pthread_kill(read_t, SIGINT);
	sigwait(&set, &sig);
	pthread_kill(calc_t, SIGINT);

	pthread_join(read_t, NULL);
	pthread_join(calc_t, NULL);

	pthread_exit(NULL);

	//close file
	fclose(val.fp);

	return(0);
}//main
