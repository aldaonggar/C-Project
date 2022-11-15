#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include <stdbool.h>
#include "sensor_db.h"
#include <time.h>

int main() {
    FILE* f = open_db("data.csv", false);
    if (f == NULL) 
        return -1;
    time_t t = time(NULL);
    if (insert_sensor(f, 1, 23.6, t) < 0)
        return -1;
    if (close_db(f))
        return -1;

}