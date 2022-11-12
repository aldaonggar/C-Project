#include <stdlib.h>
#include <stdio.h>
#include "datamgr.h"
#include "lib/dplist.h"

#define ERROR_INVALID "invalid sensor ID"
#define ERROR_MEMORY  "memory error"

dplist_t* sensor_list;

void* element_copy(void * element);
void element_free(void ** element);
int element_compare(void * x, void * y);


typedef struct {
    sensor_id_t sensor_id;         /** < sensor id */
    room_id_t room_id;
    sensor_value_t running_avg;    /** < sensor value */
    sensor_ts_t last_modified;     /** < sensor timestamp */
} sensor_list_element_t;

void * element_copy(void * element) {
    sensor_list_element_t* copy = malloc(sizeof (sensor_list_element_t));
    copy->room_id = ((sensor_list_element_t*)element)->room_id;
    copy->running_avg = ((sensor_list_element_t*)element)->running_avg;
    copy->last_modified = ((sensor_list_element_t*)element)->last_modified;
    copy->sensor_id = ((sensor_list_element_t*)element)->sensor_id;
    return (void *) copy;
}

void element_free(void ** element) {
    free(&(((sensor_list_element_t*)*element))->sensor_id);
    free(*element);
    *element = NULL;
}

int element_compare(void * x, void * y) {
    return ((((sensor_list_element_t*)x)->sensor_id < ((sensor_list_element_t*)y)->sensor_id) ? -1 : (((sensor_list_element_t*)x)->sensor_id == ((sensor_list_element_t*)y)->sensor_id) ? 0 : 1);
}

void datamgr_parse_sensor_files(FILE *fp_sensor_map, FILE *fp_sensor_data){
    
    room_id_t r_id;
    sensor_id_t s_id;
    sensor_value_t temps[1000][RUN_AVG_LENGTH];
    sensor_list = dpl_create(element_copy, element_free, element_compare);
    while (fscanf(fp_sensor_map, "%hd %hd", &r_id, &s_id)) {
         sensor_list_element_t* sensor;
         sensor = malloc(sizeof(sensor_list_element_t));
         ERROR_HANDLER(sensor == NULL, ERROR_MEMORY);
         sensor->room_id = r_id;
         sensor->sensor_id = s_id;
         for (int i = temps[s_id][0]; i < temps[s_id][RUN_AVG_LENGTH]; i++) {
            temps[s_id][i] = 0;
         }
         dpl_insert_at_index(sensor_list, sensor, 1000, false);
    }        

    int cnt[1000];
    for (int i = 0; i < 1000; i++) {
        cnt[i] = 0;
    }

    sensor_data_t* sensor_data;
    while(fread(&sensor_data, sizeof(sensor_data_t), 1, fp_sensor_data)) {
        int index = dpl_get_index_of_element(sensor_list, sensor_data);
        sensor_list_element_t* sensor = dpl_get_element_at_index(sensor_list, index);
        if (cnt[sensor_data->id] < RUN_AVG_LENGTH) {
            temps[sensor_data->id][cnt[sensor_data->id]] = sensor_data->value;
            cnt[sensor_data->id]++;
        }
        if (cnt[sensor_data->id] == RUN_AVG_LENGTH) {
            sensor_value_t avg_temp = 0;
            for (int i = 0; i < RUN_AVG_LENGTH; i++) {
                avg_temp += temps[sensor_data->id][i];
            }
            avg_temp /= RUN_AVG_LENGTH;
            sensor->running_avg = avg_temp;
            if(sensor->running_avg < SET_MIN_TEMP) {
                fprintf( stderr,"It's too cold in room %hu for sensor %hu with a running average of %lf on %ld\n", sensor->room_id, sensor->sensor_id, sensor->running_avg, sensor->last_modified);
            }
            if(sensor->running_avg > SET_MAX_TEMP) {
                fprintf( stderr, "It's too warm in room %hu for sensor %hu with a running average of %lf on %ld\n",sensor->room_id, sensor->sensor_id, sensor->running_avg, sensor->last_modified);
            }
            if (sensor->last_modified < sensor_data->ts || &sensor->last_modified == NULL) {
                sensor->last_modified = sensor_data->ts;
            } 
        }
    }
}

void datamgr_free() {
    dpl_free(&sensor_list, true);
}

uint16_t datamgr_get_room_id(sensor_id_t sensor_id) {
    for (int i = 0; i < dpl_size(sensor_list); i++) {
        sensor_list_element_t* sensor;
        sensor = dpl_get_element_at_index(sensor_list, i);
        sensor_id_t s_id = sensor->sensor_id;
        sensor_id_t r_id = sensor->room_id;
        if (s_id == sensor_id)
            return r_id;
    }
    ERROR_HANDLER(1, ERROR_INVALID);
}

sensor_value_t datamgr_get_avg(sensor_id_t sensor_id) {
    for (int i = 0; i < dpl_size(sensor_list); i++) {
        sensor_list_element_t* sensor;
        sensor = dpl_get_element_at_index(sensor_list, i);
        sensor_id_t s_id = sensor->sensor_id;
        sensor_ts_t s_run_avg = sensor->running_avg;
        if (s_id == sensor_id)
            return s_run_avg;
    }
    ERROR_HANDLER(1, ERROR_INVALID);
}

time_t datamgr_get_last_modified(sensor_id_t sensor_id) {
    for (int i = 0; i < dpl_size(sensor_list); i++) {
        sensor_list_element_t* sensor;
        sensor = dpl_get_element_at_index(sensor_list, i);
        sensor_id_t s_id = sensor->sensor_id;
        sensor_ts_t s_last_mod = sensor->last_modified;
        if (s_id == sensor_id)
            return s_last_mod;
    }
    ERROR_HANDLER(1, ERROR_INVALID);
}

int datamgr_get_total_sensors() {
    bool sensor_check[10000];
    int cnt = 0;
    for (int i = 0; i < dpl_size(sensor_list); i++) {
        sensor_list_element_t* sensor;
        sensor = dpl_get_element_at_index(sensor_list, i);
        sensor_id_t s_id = sensor->sensor_id;
        if (!sensor_check[s_id]) 
            cnt++;
        sensor_check[s_id] = true;
    }
    return cnt;
}
