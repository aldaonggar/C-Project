#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "sbuffer.h"

sem_t wrt;
pthread_mutex_t mutex;
sbuffer_t* buffer;
int cnt = 1;
int numreader = 0;
FILE* data_file;
FILE* data_out_file;

void* writer(void* writer_tid) {   
    sem_wait(&wrt);
    sbuffer_init(&buffer);
    /*
    Here I need to read from binary file, before doing it I need to test datamgr I have done.
    */
    printf("Writer %d modified cnt to %d\n",(*((int *)writer_tid)),cnt);
    sem_post(&wrt);
}

void* reader(void* reader_tid) {   
    // Reader acquire the lock before modifying numreader
    pthread_mutex_lock(&mutex);
    numreader++;
    if(numreader == 1) {
        sem_wait(&wrt); // If this id the first reader, then it will block the writer
    }
    sensor_data_t* data;
    data = sbuffer_remove(buffer, data);
    fprintf(data_out_file, "%d, %f, %ld", data->id, data->value, data->ts);
    pthread_mutex_unlock(&mutex);

    // Reader acquire the lock before modifying numreader
    pthread_mutex_lock(&mutex);
    numreader--;
    if(numreader == 0) {
        sem_post(&wrt); // If this is the last reader, it will wake up the writer.
    }
    pthread_mutex_unlock(&mutex);
}

int main() {   

    data_file = fopen("sensor_data", "r");
    data_out_file = fopen("sensor_data_out", "a");

    pthread_t reader_tid[2],writer_tid;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt,0,1);

    int a[2] = {1,2}; //Just used for numbering the producer and consumer

    pthread_create(&writer_tid, NULL, (void *)writer, (void *)&a[0]);
    for(int i = 0; i < 2; i++) {
        pthread_create(&reader_tid[i], NULL, (void *)reader, (void *)&a[i]);
    }
    
    pthread_join(writer_tid, NULL);
    for(int i = 0; i < 2; i++) {
        pthread_join(reader_tid[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    sbuffer_free(&buffer);
    
    fclose(data_file);
    fclose(data_out_file);
    
    return 0;   
}