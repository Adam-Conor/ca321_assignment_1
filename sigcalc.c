#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

#define SLEEP 10000

int x;
int y;
sigset_t set;
int sig;

static void * reader(void *in);
static void * calculator(void *in);
static void * control(void *in);

typedef struct {
	int x;
	int y;
	FILE *fp;
} values_t;

static void * reader(void *in) {
	//char** argv = in;	
	//FILE *fp; //var for file
	sigset_t set;
	int sig;

	sigemptyset(&set);
	sigset(SIGUSR1, calculator);

	//fp = fopen(argv[1], "r"); //open file

	/*if(fp == NULL) { //check for valid file
		perror("error opening file");
		return((void *)NULL);
	}
	*/

	while(!feof(fp)) {
		fscanf(fp, "%d %d", &x, &y); //get two ints

		printf("Thread 1 submitting : %d %d\n", x, y); //print out the ints

		//signal to go here
		pthread_cond_signal(&sig);

		sigwait(&set, &sig);

		usleep(SLEEP);
	}

	//fclose(fp); //close the file
	return((void *)NULL);
}

static void * calculator(void *in) {
	while(1) {
		sigwait(&set, &sig);

		printf("Thread 2 calculated : %d\n", x + y);

		//signal to go here
		pthread_cond_signal(&sig);

		usleep(SLEEP);
	}

	return((void *)NULL);
}

static void * control(void *in) {
	char** argv = in;
	values_t val;

	val.fp = fopen(argv[1], "r");

	if(fp == NULL) {
			perror("error opening file");
			return((void *)NULL);
	}

	pthread_t read_t; //init threads
	pthread_t calc_t;

	pthread_create(&read_t, NULL, &reader, (void *)&val); //create threads
	pthread_create(&calc, NULL, &reader, (void *)&val); //create threads

	fclose(val.fp);
	return((void *)NULL);
}

int main(int arc, char *argv[]) {
	pthread_t cont_t; //init threads
	//pthread_t read_t; //init threads
	//pthread_t calc_t;

	//pthread_create(&read, NULL, &reader, (void *)argv); //create threads
	//pthread_create(&calc, NULL, &calculator, (void *)NULL);
	pthread_create(&cont_t, NULL, &reader, (void *)argv); //create threads

	//pthread_join(read_t, NULL); //starts threads
	//pthread_join(calc_t, NULL);
	pthread_join(cont_t, NULL);
	

	return(0);
}
