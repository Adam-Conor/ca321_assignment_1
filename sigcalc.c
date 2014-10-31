#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

#define SLEEP 10000

static void * reader(void *in);
static void * calculator(void *in);
static void * control(void *in);

//struct to hold data
typedef struct {
	int x;
	int y;
	FILE *fp;
} values_t;

static void * reader(void *val_in) {
	values_t *val = val_in;
	sigset_t set;
	
	//set signal
	sigemptyset(&set);
	//sigaddset(&set,SIGALRM);
	sigset(SIGINT, calculator);
	//sigsuspend(&set);

	//alarm(7); causing seg fault

	while(1) {//!feof(val->fp)) {
		//read in the two ints
		fscanf(val->fp, "%d %d", &val->x, &val->y);
		
		//print out valuesto be calculated
		printf("Thread 1 submitting : %d %d\n", val->x, val->y);

		//signal(SIGUSR1, NULL);
		kill(getpid(),SIGINT);
		usleep(SLEEP);
		//sigwait(&set, &sig);
		sigsuspend(&set);
		//sleep
		//usleep(SLEEP);
	}

	return((void *)NULL);
}//reader

static void * calculator(void *val_in) {
	values_t *val = val_in;
	sigset_t set;

	//set signal
	sigemptyset(&set);
	//sigaddset(&set, SIGALRM);
	sigset(SIGINT, reader);

	while(1) {
		//wait for signal
		//sigwait(&set, &sig);
		sigsuspend(&set);
		
		//calculate 
		printf("Thread 2 calculated : %d\n", val->x + val->y);
		//sigwait(&set, &sig);

		//signal(SIGUSR1, NULL);
		//alarm(1);
		sigsuspend(&set);
		//sleep
		usleep(SLEEP);
	}

	return((void *)NULL);
}//calculator

static void * control(void *in) {
	char** argv = in;
	values_t val;

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
