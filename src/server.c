#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include "server.h"


// messages will be received as a receive_t
typedef union {
	uint16_t type;
	struct _pulse pulse;
	distribute_water_t distribute_water;
	check_humidity_t check_humidity;
	check_temperature_t check_temperature;
	change_light_t change_light;
} receive_t;


// function declarations go up here
int adjustNumInRange(int value, int low, int high);
int changeLights(int status);
int activateSprinklers();
int adjustTempByTimeOfDay(int temp);


int main(void) {

	puts("Starting Greenhouse Monitoring System Simulation...");

	// define variables
	int rcvid;
	name_attach_t *attach;

	// define message variables
	receive_t msg;
	struct _msg_info info;
	distribute_water_resp_t distribute_water_resp;
	check_humidity_resp_t check_humidity_resp;
	check_temperature_resp_t check_temperature_resp;
	change_light_resp_t change_light_resp;

	// register name and create a channel
	if ((attach = name_attach(NULL, GREENHOUSE_SERVER_NAME, 0)) == NULL) {
		return EXIT_FAILURE;
	}

	while (1) {
		// receive a message
		rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), &info);

		if (0 == rcvid) {
			// received a pulse
			switch (msg.pulse.code) {
			case _PULSE_CODE_DISCONNECT:
				printf("disconnect pulse received\n");
				if (-1 == ConnectDetach(msg.pulse.scoid)) {
					perror("ConnectDetach");
				}
			break;

			default:
				printf("unknown pulse with code = %d received\n", msg.pulse.code);
			}
		} else {
			// received a message -- handle message based on type
			switch (msg.type) {
			case DISTRIBUTE_WATER_MSG_TYPE:
				printf("distribute_water message received\n");
				// sample usage of the activateSprinklers function
				// distribute_water_resp.status = activateSprinklers();
				MsgReply(rcvid, 0, &distribute_water_resp, sizeof(distribute_water_resp));
				break;
			case CHECK_HUMIDITY_MSG_TYPE:
				printf("check_humidity message received\n");
				// sample usage of the adjustNumInRange function
				// check_humidity_resp.updated_humidity_level = adjustNumInRange(msg.check_humidity.humidity_level, 50, 90);
				MsgReply(rcvid, 0, &check_humidity_resp, sizeof(check_humidity_resp));
				break;
			case CHECK_TEMP_MSG_TYPE:
				printf("check_temperature message received\n");
				// sample usage of the adjustTempByTimeOfDay function
				// check_temperature_resp.updated_temp = adjustTempByTimeOfDay(msg.check_temperature.temp);
				MsgReply(rcvid, 0, &check_temperature_resp, sizeof(check_temperature_resp));
				break;
			case CHANGE_LIGHT_MSG_TYPE:
				printf("change_light message received\n");
				// sample usage of the changeLights function
				// change_light_resp.updated_light_status = changeLights(msg.change_light.light_status);
				MsgReply(rcvid, 0, &change_light_resp, sizeof(change_light_resp));
				break;
			default:
				printf("unknown message received\n");
				break;
			}
		}
	}

	return EXIT_SUCCESS;
}

// functions for handling individual components can go down here

/**
 * Range Helper Function - many of the clients will require some way to determine if a value is outside
 * of a given range, and then adjust that value to be within the range if needed. This function can be
 * used in those cases
 */
int adjustNumInRange(int value, int low, int high) {
	if (value > high) {
		printf("Value is too high (%d)! Lowering to %d...\n", value, high);
		return high;
	} else if (value < low) {
		printf("Value is too low (%d)! Raising to %d...\n", value, low);
		return low;
	}
	return value;
}

/**
 * Adjusts light status
 * 		lights on (true)   -> lights off (false)
 * 		lights off (false) -> lights on (true)
 */
int changeLights(int status) {
	return (status == FALSE) ? TRUE: FALSE;
}

/**
 * Simulates sprinklers being turned on, running for 2 hours (2 seconds for the simulation),
 * and then returning
 */
int activateSprinklers() {
	printf("Sprinklers Activated\n");
	sleep(2);
	return 0;
}


/**
 * Valid temperature is dependent on the time of day (light status)
 */
int adjustTempByTimeOfDay(int temp) {
	// NOTE --> need some way to store the current time of day (global var? shared mem?)
	if (1) {
		// during the day
		return adjustNumInRange(temp, 24, 30);
	} else {
		// at night
		return adjustNumInRange(temp, 15, 24);
	}
}
