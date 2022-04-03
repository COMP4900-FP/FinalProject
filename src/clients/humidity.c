#include <stdio.h>
#include <stdlib.h>
#include "server.h"

int loop_humidity(int);

int loop_humidity(int chid){
    printf("Humidity - Started (%d)\n", chid);
    check_humidity_t req = {.type = CHECK_TEMP_MSG_TYPE};
    check_humidity_resp_t res;
	MsgSend(chid, &req, sizeof(req), &res, sizeof(res));
    return EXIT_SUCCESS;
}   
