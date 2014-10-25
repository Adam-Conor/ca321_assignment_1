#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

#define SLEEP 10000

int x;
int y;
bool endOfFile;
sigset_t set;
int sig;

static void * reader(void *in) {
	char** argv = in;	
	FILE *fp; //var for file

	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);

	fp = fopen(argv[1], "r"); //open file

	if(fp == NULL) { //check for valid file
		perror("error opening file");
		return((void *)NULL);
	}

	while(!feof(fp)) {
		fscanf(fp, "%d %d", &x, &y); //get two ints

		printf("Thread 1 submitting : %d %d\n", x, y); //print out the ints

		//signal to go here

		//sigwait(&set, &sig);

		usleep(SLEEP);
	}

	endOfFile = true;

	fclose(fp); //close the file
	return((void *)NULL);
}

static void * calculator(void *in) {
	while(!endOfFile) {
		//sigwait(&set, &sig);

		printf("Thread 2 calculated : %d\n", x + y);

		//signal to go here

		usleep(SLEEP);
	}

	return((void *)NULL);
}


int main(int arc, char *argv[]) {
	pthread_t read_t; //init threads
	pthread_t calc_t;

	pthread_create(&read, NULL, &reader, (void *)argv); //create threads
	pthread_create(&calc, NULL, &calculator, (void *)NULL);

	pthread_join(read_t, NULL); //starts threads
	pthread_join(calc_t, NULL);

	return(0);
}
