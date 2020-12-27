#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

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
        char buf[1024];
        int len;
        close(pipedes[1]);
        while ((len = read(pipedes[0], buf, 1024)) != 0)
            write(2, buf, len);
        close(pipedes[0]);
        printf("Child time---> %s", asctime(timeinfo));
        exit(0);
    } else {
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        printf("This is a parent process\n");
        char* pidstr;
        int pidInt = getpid();
        itoa(pidInt, pidstr, 10);
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