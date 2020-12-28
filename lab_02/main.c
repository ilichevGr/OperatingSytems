#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUF_SIZE 1024

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
    int pipedes[2];
    pid_t pid;
    pipe(pipedes);
    time_t rawtime;
    struct tm * timeinfo;
    char* str = " <--- Parent time || Parent PID ---> ";

    pid = fork();
    if (pid == 0) {
        sleep(5);
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        printf("This is a child process\n");
        char buf[BUF_SIZE];
        int len;
        close(pipedes[1]);
        while ((len = read(pipedes[0], buf, BUF_SIZE)) != 0)
            write(2, buf, len);
        close(pipedes[0]);
        printf("Child time---> %s", asctime(timeinfo));
        exit(0);
    } else {
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        printf("This is a parent process\n");  
        int pidInt = getpid();
        char* pidstr = itoa(pidInt, 10);
        char* message = sumStr(asctime(timeinfo), str, pidstr, "\n");
        //printf("%s",message);
        close(pipedes[0]);
        write(pipedes[1], message, strlen(message) + 1);
        close(pipedes[1]);
        wait(0);
        exit(0);
    }
    return 0;
}
