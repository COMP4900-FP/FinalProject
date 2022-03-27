#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include <unistd.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>


// messages will be received as a receive_t
typedef union {
	uint16_t type;
	struct _pulse pulse;
	distribute_water_t distribute_water;
	check_humidity_t check_humidity;
	check_temperature_t check_temperature;
	change_light_t change_light;
} receive_t;


int main(void) {

	// register name and create a channel
	name_attach_t *attach;
	if ((attach = name_attach(NULL, GREENHOUSE_SERVER_NAME, 0)) == NULL) {
		return EXIT_FAILURE;
	}

	puts("Done!");
	return EXIT_SUCCESS;

}
