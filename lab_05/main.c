#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define TRUE 1
#define ARRAY_SIZE 100

int threadsCount = 11;
int recordNo = 0;
int buf[ARRAY_SIZE];

pthread_mutex_t mutex;

void* reader(){
    while(TRUE){
        pthread_mutex_lock(&mutex);
        printf("TID: %d\nARRAY LENGTH: %d\n",(int)pthread_self(),buf[recordNo]);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}

void* writer(){
    while(TRUE){
        pthread_mutex_lock(&mutex);
        recordNo++;
        if (recordNo < ARRAY_SIZE) buf[recordNo] = recordNo;
        printf("RECORD NO: %d\n",recordNo);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}

int main(void){
    pthread_t threads[threadsCount];
    pthread_create(&threads[0], NULL, writer, NULL);
    for (int i = 1; i < threadsCount; i++) pthread_create(&threads[i], NULL, reader,NULL);
    for (int i = 0; i < threadsCount; i++) pthread_join(threads[i], NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}