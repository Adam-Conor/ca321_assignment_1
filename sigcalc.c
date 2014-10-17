#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

static void *
reader(void *in) {
	char** argv = in;	
	FILE *fp; //var for file
	int x; //vars for numbers to add
	int y;
	//sigset_t set;
	//int sig;

	//sigemptyset(&set);
	//sigaddset(&set, SIGUSR1);

	fp = fopen(argv[1], "r"); //open file

	if(fp == NULL) { //check for valid file
		perror("error opening file");
		return((void *)NULL);
	}

	while(1) {
		fscanf(fp, "%d %d", &x, &y); //get two ints

		if(feof(fp)) { //check for end of file
			break;
		}

		printf("%d %d\n", x, y); //print out the ints
		
		//sigwait(&set, &sig);
	}

	fclose(fp); //close the file
	return((void *)NULL);
}


int main(int arc, char *argv[]) {
	pthread_t read;

	pthread_create(&read, NULL, &reader, (void *)argv);

	pthread_join(read, NULL);

	return(0);
}
