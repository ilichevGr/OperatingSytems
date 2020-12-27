#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define FILE_PATH "client.c"
#define IPC 17

typedef struct data {
	char time[100];
	pid_t pid;
} data;

int ID;

char* itoa(int value, char* result, int base) {
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

void destructor(int signal){
	struct shmid_ds *buf = 0;
	shmctl(ID, IPC_RMID, buf);
	exit(0);
}

int main(void){
	
	signal(SIGINT, destructor);

	key_t key = ftok(FILE_PATH, IPC);
	time_t rawtime;
	struct tm * timeinfo;
	data *dataset;


	if (key == -1){
		printf("KEY ERRNO: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	ID = shmget(key, sizeof(data), IPC_CREAT | 0666);

	if (ID == -1){
		printf("SHM ERRNO: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	dataset = (data*) shmat(ID, NULL, 0);

	if (dataset == (data*) -1){
		printf("SHMAT ERRNO: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	if(dataset->pid != 0){
		printf("Process is already started!\n");
		exit(0);
	}


	while(1){
		sleep(3);
		data datatmp;
		time(&rawtime);
	        timeinfo = localtime(&rawtime);
		strcpy(datatmp.time, asctime(timeinfo));
		strcat(datatmp.time, "\0");

		datatmp.pid = getpid();
		printf("Data sent!\n");
		*dataset = datatmp;
	}

}	
