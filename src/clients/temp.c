#include <stdio.h>
#include <stdlib.h>
#include "server.h"

int *loop_temp(int);

int *loop_temp(int chid){
    printf("Temp - Started (%d)\n", chid);
    check_temperature_t req = {.type = CHECK_TEMP_MSG_TYPE};
    check_temperature_resp_t res;
	MsgSend(chid, &req, sizeof(req), &res, sizeof(res));
    return EXIT_SUCCESS;
}
