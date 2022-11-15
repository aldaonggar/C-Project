#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "lib/dplist.h"
#include "datamgr.h"
#include <time.h>

int main(){
    printf("Hello World\n");
    printf("s");
    FILE * map = fopen("/home/nur/labOS/nur.aldaonggar/room_sensor.map", "r");
    FILE * data = fopen("/home/nur/labOS/nur.aldaonggar/sensor_data", "rb");
    printf("SIU");
    if(map == NULL) return -1;
    if(data == NULL) return -1;

    datamgr_parse_sensor_files(map, data);
    printf("%f", datamgr_get_avg(21));
    printf("%ld", datamgr_get_last_modified(21));
    printf("%hd", datamgr_get_room_id(21));
    printf("%d", datamgr_get_total_sensors());
    datamgr_free();

    fclose(map);
    fclose(data);

}