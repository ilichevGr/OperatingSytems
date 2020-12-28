#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define TRUE 1
#define ARRAY_SIZE 100

int threadsCount = 11;
int recordNo = 0;
int buf[ARRAY_SIZE];

pthread_mutex_t mutex;
pthread_cond_t pthreadCond;

void* reader(){
    while(TRUE){
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&pthreadCond, &mutex);
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
        int rndm = rand() % 100;
        if (rndm < 50) pthread_cond_broadcast(&pthreadCond);
        else pthread_cond_signal(&pthreadCond);
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 5 + 1);
    }
}

int main(void){
    pthread_t threads[threadsCount];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&pthreadCond, NULL);
    pthread_create(&threads[0], NULL, writer, NULL);
    for (int i = 1; i < threadsCount; i++) pthread_create(&threads[i], NULL, reader,NULL);
    for (int i = 0; i < threadsCount; i++) pthread_join(threads[i], NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}
