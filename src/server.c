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
				MsgReply(rcvid, 0, &distribute_water_resp, sizeof(distribute_water_resp));
				break;
			case CHECK_HUMIDITY_MSG_TYPE:
				printf("check_humidity message received\n");
				MsgReply(rcvid, 0, &check_humidity_resp, sizeof(check_humidity_resp));
				break;
			case CHECK_TEMP_MSG_TYPE:
				printf("check_temperature message received\n");
				MsgReply(rcvid, 0, &check_temperature_resp, sizeof(check_temperature_resp));
				break;
			case CHANGE_LIGHT_MSG_TYPE:
				printf("change_light message received\n");
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
