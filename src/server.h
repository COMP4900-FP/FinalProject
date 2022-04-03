#include <stdint.h>
#include <sys/iomsg.h>
#include <sys/mman.h>
#include <pthread.h>

#define GREENHOUSE_SERVER_NAME "greenhouse_server"

#define DISTRIBUTE_WATER_MSG_TYPE (_IO_MAX+200)
#define CHECK_HUMIDITY_MSG_TYPE (_IO_MAX+201)
#define CHECK_TEMP_MSG_TYPE (_IO_MAX+202)
#define CHANGE_LIGHT_MSG_TYPE (_IO_MAX+203)

#define TRUE 1
#define FALSE 0

#define lockShmem(A) (pthread_mutex_lock(&(A)->dataMutex))
#define unlockShmem(A) (pthread_mutex_unlock(&(A)->dataMutex))


typedef struct shared_data {
	pthread_mutex_t dataMutex;
	pthread_cond_t  dataCondVar;
	struct {
		uint8_t light;
	} lightData;
	struct {
		int targetSaturation;
	} soilData;
	struct {
		int targetTemp;
	} tempData;
	struct {
		int targetHumidity;
	} humidityData;
} shared_data_t;


// water distribution
typedef struct distribute_water {
	uint16_t type;
	uint8_t saturation;
} distribute_water_t;

typedef struct distribute_water_resp {
	uint8_t targetSaturation;
} distribute_water_resp_t;


// humidity
typedef struct check_humidity {
	uint16_t type;
	int humidity_level;
} check_humidity_t;

typedef struct check_humidity_resp {
	int updated_humidity_level;
} check_humidity_resp_t;


// temperature
typedef struct check_temperature {
	uint16_t type;
	int temp;
	int light;
} check_temperature_t;

typedef struct check_temperature_resp {
	int updated_temp;
} check_temperature_resp_t;


// light
typedef struct change_light {
	uint16_t type;
	int light_status;
	int hourOfDay;
} change_light_t;

typedef struct change_light_resp {
	int updated_light_status;
} change_light_resp_t;
