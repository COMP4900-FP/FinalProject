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


// function declarations
int adjustNumInRange(int value, int low, int high);
int adjustTempByTimeOfDay(int temp, int light);


int main(void) {

	puts("Starting Greenhouse Monitoring System Simulation...");

	// define variables
	int rcvid, hod;
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
				printf("DISCONNECT received\n");
				if (-1 == ConnectDetach(msg.pulse.scoid)) {
					perror("ConnectDetach");
				}
			break;

			default:
				printf("UNKNOWN PULSE, code = %d\n", msg.pulse.code);
			}
		} else {
			// received a message -- handle message based on type
			switch (msg.type) {
			case DISTRIBUTE_WATER_MSG_TYPE:
				printf("DIST WATER RECEIVED: (%d)\n", msg.distribute_water.saturation);
				// update saturation level
				distribute_water_resp.saturation = adjustNumInRange(msg.distribute_water.saturation, 50, 90);
				MsgReply(rcvid, 0, &distribute_water_resp, sizeof(distribute_water_resp));
				break;
			case CHECK_HUMIDITY_MSG_TYPE:
				printf("CHK HUMIDITY RECEIVED: (%d)\n", msg.check_humidity.humidity_level);
				// update humidity level
				check_humidity_resp.updated_humidity_level = adjustNumInRange(msg.check_humidity.humidity_level, 50, 90);
				MsgReply(rcvid, 0, &check_humidity_resp, sizeof(check_humidity_resp));
				break;
			case CHECK_TEMP_MSG_TYPE:
				printf("CHK TEMP RECEIVED: (%d)\n", msg.check_temperature.temp);
				// update temperature depending on the hour of day
				check_temperature_resp.updated_temp = adjustTempByTimeOfDay(msg.check_temperature.temp, msg.check_temperature.light);
				MsgReply(rcvid, 0, &check_temperature_resp, sizeof(check_temperature_resp));
				break;
			case CHANGE_LIGHT_MSG_TYPE:
				hod = msg.change_light.hourOfDay;
				printf("CHG LIGHT RECEIVED: (HOD: %d)\n", hod);
				// update lights depending on the hour of day
				change_light_resp.updated_light_status = hod >= 7 && hod  <= 18;
				MsgReply(rcvid, 0, &change_light_resp, sizeof(change_light_resp));
				break;
			default:
				printf("UNKWN MESSAGE\n");
				break;
			}
		}
	}

	return EXIT_SUCCESS;
}


/**
 * Range Helper Function
 */
int adjustNumInRange(int value, int low, int high) {
	int newValue = (high - low)/2 + low;
	if (value > high) {
		printf("Value is too high (%d)! Lowering to %d...\n", value, newValue);
		return newValue;
	} else if (value < low) {
		printf("Value is too low (%d)! Raising to %d...\n", value, newValue);
		return newValue;
	}
	// if values are unchanged, return -1
	return -1;
}


/**
 * Valid temperature range is dependent on the time of day (light status)
 */
int adjustTempByTimeOfDay(int temp, int light) {
	if (light == 1) {
		// during the day
		return adjustNumInRange(temp, 24, 30);
	} else {
		// at night
		return adjustNumInRange(temp, 15, 24);
	}
}
