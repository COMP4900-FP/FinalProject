#include <stdlib.h>
#include <stdio.h>
#include "server.h"


int loop_soil(int);

int loop_soil(int chid){
    printf("Soil - Started (%d)\n", chid);
    distribute_water_t req = {.type = DISTRIBUTE_WATER_MSG_TYPE};
    distribute_water_resp_t res;
	MsgSend(chid, &req, sizeof(req), &res, sizeof(res));
    return EXIT_SUCCESS;
}