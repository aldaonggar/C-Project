#include <stdlib.h>
#include "datamgr.h"
#include "lib/dplist.h"

dplist_t* sensor_list;

typedef struct {
    sensor_id_t sensor_id;         /** < sensor id */
    int room_id;
    sensor_value_t running_avg;   /** < sensor value */
    sensor_ts_t last_modified;         /** < sensor timestamp */
} sensor_list_element_t;

