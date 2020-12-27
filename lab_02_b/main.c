#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>



char* itoa(int val, int base){
    static char buf[32] = {0};
    int i = 30;
    for(; val && i ; --i, val /= base)
        buf[i] = "0123456789abcdef"[val % base];
    return &buf[i+1];

}

char* sumStr(char* str1, char* str2, char* str3, char* str4){
    char* sum = malloc(strlen(str1) + strlen(str2) + strlen(str3) + strlen(str4) + 1);
    strcpy(sum, str1);
    strcat(sum, str2);
    strcat(sum, str3);
    strcat(sum, str4);
    return sum;
}

int main (int argc, char * argv[]) {
    int fd_fifo;
    char buff[100];
    unlink("/tmp/fifo1");
    if((mkfifo("/tmp/fifo1",0666)) == -1){
        printf("Cant create FIFO\n");
        exit(0);
    }
    pid_t pid;
    time_t rawtime;
    struct tm * timeinfo;
    char* str = " <--- Parent time || Parent PID ---> ";

    pid = fork();
    if (pid == 0) {
        sleep(5);
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        printf("This is a child process\n");
        if ((fd_fifo=open("/tmp/fifo1",O_RDONLY)) == -1){
            printf("Cant open fifo\n");
            exit(0);
        }
        read(fd_fifo, &buff, sizeof(buff));
        close(fd_fifo);
        printf("%s",buff);
        fflush(stdout);
        printf("Child time---> %s", asctime(timeinfo));
        exit(0);
    } else {
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        printf("This is a parent process\n");
        int pidInt = getpid();
        char* pidstr = itoa(pidInt, 10);
        char* message = sumStr(asctime(timeinfo), str, pidstr, "\n \0");
        if ((fd_fifo=open("/tmp/fifo1",O_WRONLY)) == -1){
            printf("Cant open fifo\n");
            exit(0);
        }
        write(fd_fifo, message, strlen(message));
        close(fd_fifo);
        wait(0);
        exit(0);
    }
    return 0;
}