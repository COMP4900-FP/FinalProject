#include <stdio.h>
#include <stdlib.h>
#include "server.h"

int loop_lights(int);

int loop_lights(int chid){
    printf("Light - Started (%d)\n",chid);
    change_light_t req = {.type = CHANGE_LIGHT_MSG_TYPE};
    change_light_resp_t res;
	MsgSend(chid, &req, sizeof(req), &res, sizeof(res));
    return EXIT_SUCCESS;
}
