#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

int x;
int y;
bool endOfFile;
sigset_t set;
int sig;

static void *
reader(void *in) {
	char** argv = in;	
	FILE *fp; //var for file
	//int x; //vars for numbers to add
	//int y;
	//sigset_t set;
	//int sig;

	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);

	fp = fopen(argv[1], "r"); //open file

	if(fp == NULL) { //check for valid file
		perror("error opening file");
		return((void *)NULL);
	}

	while(1) {
		fscanf(fp, "%d %d", &x, &y); //get two ints

		if(feof(fp)) { //check for end of file
			endOfFile = true;
			break;
		}

		printf("%d %d\n", x, y); //print out the ints
		/** TODO **/
		//signal to calc
		//sig
		usleep(1000);

		/*signal(SIGUSR1, &calculator);
		sigwait(&set, &sig);
		sleep(1);
		*/
	}

	fclose(fp); //close the file
	return((void *)NULL);
}

static void *
calculator(void *in) {
	//sigemptyset(&set);
	//sigaddset(&set, SIGUSR1);

	while(!endOfFile) {
		/** TODO **/
		//wait for signal
		//sigwait(&set, &sig);
		printf("%d\n", x + y);
		//signal(SIGUSR1, &reader);
		/** TODO **/
		//signal reader
		usleep(1000);
	}
	return((void *)NULL);
}


int main(int arc, char *argv[]) {
	pthread_t read; //init threads
	pthread_t calc;

	pthread_create(&read, NULL, &reader, (void *)argv); //create threads
	pthread_create(&calc, NULL, &calculator, (void *)NULL);

	pthread_join(read, NULL); //starts threads
	pthread_join(calc, NULL);

	return(0);
}
