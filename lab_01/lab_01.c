#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

void exitMessage(){
	printf("Atexit!\nPID: %d\n", getpid());
}


int main (void){
	pid_t pid;
	
	atexit(exitMessage);
	pid = fork();
	
	switch(pid){
		case -1:
			perror("Memory error\n");
			exit(0);
		case 0:
			printf("This is a child proccess\n");
			printf("PID: %d\n", getpid());
			printf("PPID: %d\n", getppid());
			
			exit(0);
		default: 
			printf("This is a parent proccess\n");
			printf("PID: %d\n", getpid());
			printf("PPID: %d\n", getppid());
			wait(0);

			exit(0);
	}
	return 0;
}
