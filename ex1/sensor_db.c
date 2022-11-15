#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include <stdbool.h>
#include "sensor_db.h"

FILE* open_db(char* filename, bool append) {
    FILE* data;
    if (append) {
        data = fopen(filename, "a");
    } else {
        data = fopen(filename, "w");
    }
    return data;
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts) {
    return fprintf(f, "%d, %f, %ld\n", id, value, ts);
}

int close_db(FILE * f) {
    return fclose(f);
}