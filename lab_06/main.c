#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define TRUE 1
#define ARRAY_SIZE 100

int threadsCount = 11;
int recordNo = 0;
int buf[ARRAY_SIZE];

pthread_rwlock_t rwlock;

void* reader(){
    while(TRUE){
        pthread_rwlock_rdlock(&rwlock);
        printf("TID: %d\nARRAY LENGTH: %d\n",(int)pthread_self(),buf[recordNo]);
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
    }
}

void* writer(){
    while(TRUE){
        pthread_rwlock_wrlock(&rwlock);
        recordNo++;
        if (recordNo < ARRAY_SIZE) buf[recordNo] = recordNo;
        printf("RECORD NO: %d\n",recordNo);
        pthread_rwlock_unlock(&rwlock);
        sleep(rand() % 5 + 1);
    }
}

int main(void){
    pthread_t threads[threadsCount];
    pthread_rwlock_init (&rwlock, NULL);
    pthread_create(&threads[0], NULL, writer, NULL);
    for (int i = 1; i < threadsCount; i++) pthread_create(&threads[i], NULL, reader,NULL);
    for (int i = 0; i < threadsCount; i++) pthread_join(threads[i], NULL);
    pthread_rwlock_destroy(&rwlock);
    return 0;
}
