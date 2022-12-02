#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "sbuffer.h"

sem_t wrt;
pthread_mutex_t mutex;
sbuffer_t* buffer;
int num_reader = 0;
FILE* data_file;
FILE* data_out_file;

void* writer(void* writer_tid) {   
    sem_wait(&wrt);

    sensor_data_t* data = malloc(sizeof(sensor_data_t));
    sensor_id_t sensor_id;
    sensor_value_t temperature;
    sensor_ts_t time;

    while(fread(&sensor_id, sizeof(sensor_id_t), 1, data_file)) {
        data->id = sensor_id;
        fread(&temperature, sizeof(sensor_value_t), 1, data_file);
        data->value = temperature;
        fread(&time,sizeof(sensor_ts_t),1,data_file);
        data->ts = time;
        sbuffer_insert(buffer, data);
    }

    data->id = 0;
    data->value = 0;
    data->ts = 0;

    sbuffer_insert(buffer, data);

    sem_post(&wrt);
}

void* reader(void* reader_tid) {   
    pthread_mutex_lock(&mutex);
    num_reader++;
    if(num_reader == 1) {
        sem_wait(&wrt);
    }

    sensor_data_t* data = malloc(sizeof(sensor_data_t));
    sbuffer_remove(buffer, data);
    
    while (data->id != 0) {
        if (data->id != 0)
            fprintf(data_out_file, "%d, %f, %ld \n", data->id, data->value, data->ts);    
        sbuffer_remove(buffer, data);   
    }
    
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    num_reader--;
    if(num_reader == 0) {
        sem_post(&wrt); 
    }
    pthread_mutex_unlock(&mutex);
}

int main() {   

    data_file = fopen("sensor_data", "r");
    data_out_file = fopen("sensor_data_out.csv", "a");
    
    sbuffer_init(&buffer);

    pthread_t reader_tid[2],writer_tid;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt,0,1);

    int a[2] = {1, 2}; 

    pthread_create(&writer_tid, NULL, &writer, (void *)&a[0]);
    pthread_join(writer_tid, NULL);

    for(int i = 0; i < 2; i++) {
        pthread_create(&reader_tid[i], NULL, &reader, (void *)&a[i]);
    }
    
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