#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

void errMessage(){
	printf("Error! Atexit\n");
}

void parentExitMessage(){
	printf("Parent proccess stopped successfully! Atexit\n");
}

void childExitMessage(){
	printf("Child proccess stopped successfully! Atexit\n");
}


int main (void){
	pid_t pid;

	pid = fork();
	
	switch(pid){
		case -1:
			perror("Memory error\n");
			atexit(errMessage);
			exit(0);
		case 0:
			printf("This is a child proccess\n");
			printf("PID: %d\n", getpid());
			printf("PPID: %d\n", getppid());
			
			atexit(childExitMessage);
			exit(0);
		default: 
			printf("This is a parent proccess\n");
			printf("PID: %d\n", getpid());
			printf("PPID: %d\n", getppid());
			wait(0);

			atexit(parentExitMessage);
			exit(0);
	}
	return 0;
}
