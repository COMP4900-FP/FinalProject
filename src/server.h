#include <stdint.h>
#include <sys/iomsg.h>
#include <sys/mman.h>


#define GREENHOUSE_SERVER_NAME "greenhouse_server"


// water distribution
typedef struct distribute_water {
	uint16_t type;
} distribute_water_t;

typedef struct distribute_water_resp {
	int placeholder;
} distribute_water_resp_t;


// humidity
typedef struct check_humidity {
	uint16_t type;
} check_humidity_t;

typedef struct check_humidity_resp {
	int placeholder;
} check_humidity_resp_t;


// temperature
typedef struct check_temperature {
	uint16_t type;
} check_temperature_t;

typedef struct check_temperature_resp {
	int placeholder;
} check_temperature_resp_t;


// light
typedef struct change_light {
	uint16_t type;
} change_light_t;

typedef struct change_light_resp {
	int placeholder;
} change_light_resp_t;
