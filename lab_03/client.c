#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define FILEPATH "client.c"
#define TRUE 1
#define IPC 17

typedef struct data {
	char time[100];
	pid_t pid;
} data;

int ID;

void destructor(int signal){
	exit(0);
}

int main(void) {
	signal(SIGINT, destructor);

	time_t rawtime;
	struct tm * timeinfo;

	key_t key = ftok(FILEPATH, IPC);

	if (key == -1) {
		printf("KEY ERRNO: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	ID = shmget(key, sizeof(data), 0666 | IPC_EXCL);
	
	if (ID == -1) {
		printf("SHM ERRNO: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	data* dataget = (data*) shmat (ID, NULL, 0);
	if (dataget == (data*) -1) {
		printf("SHMAT ERRNO: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	while(TRUE){
		data currdata;
		time(&rawtime);
	        timeinfo = localtime(&rawtime);

		strcpy(currdata.time,asctime(timeinfo));
		currdata.pid = getpid();

		printf("PID of the client: %d\n Current time: %s\n PID of the server: %d\n Time of the server: %s\n",currdata.pid, currdata.time, dataget->pid, dataget->time);

		sleep(1);
	}
}		
