#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/sem.h>

#define FILEPATH "client.c"
#define TRUE 1
#define IPC 17

typedef struct data {
	char time[100];
	pid_t pid;
} data;

struct sembuf semw = {0, -1, SEM_UNDO};
struct sembuf sems = {0, 1, SEM_UNDO};
int ID;
int semID;

void destructor(int signal){
	struct shmid_ds *buf = 0;
	shmctl(ID, IPC_RMID, buf);
	exit(0);
}

int main(void){
	
	signal(SIGINT, destructor);

	key_t key = ftok(FILEPATH, IPC);
	time_t rawtime;
	struct tm * timeinfo;
	data *dataset;


	if (key == -1){
		printf("KEY ERRNO: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	ID = shmget(key, sizeof(data), IPC_EXCL | 0777);

	if (ID == -1){
		printf("SHM ERRNO: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	dataset = (data*) shmat(ID, NULL, 0);

	if (dataset == (data*) -1){
		printf("SHMAT ERRNO: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	key_t semKey = ftok("semName", 200);
	semID = semget(semKey, 1, 0777 | IPC_CREAT);
	
	if (semID == -1){
		printf("SEMID ERRNO: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	while(TRUE){
		sleep(1);		
		data datatmp;
		time(&rawtime);
	    timeinfo = localtime(&rawtime);
		strcpy(datatmp.time, asctime(timeinfo));
		strcat(datatmp.time, "\0");
		datatmp.pid = getpid();		
		
		printf("CLIENT PID: %d\nCLIENT TIME: %s\nSERVER PID: %d\nSERVER TIME: %s\n",datatmp.pid, datatmp.time, dataset->pid, dataset->time);
		
		
		int semSignal = semop(semID, &sems, 1);
		if(semSignal == -1){
			printf("SEM_UNLOCK ERRNO: %d\n", errno);
			exit(EXIT_FAILURE);
		}

	}
	return 0;	
}
