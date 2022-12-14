#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include <stdbool.h>

FILE* open_db(char* filename, bool append);
int insert_sensor(FILE* f, sensor_id_t id, sensor_value_t value);
int close_db(FILE * f);
void insert_to_log(char* message);
FILE * open_log();
int close_log();
void child_process(char* message);
