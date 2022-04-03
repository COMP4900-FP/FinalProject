#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include <unistd.h>

int loop_lights(int, void*);

int loop_lights(int chid, void* data){
    shared_data_t* shmem = (shared_data_t*) data;
    printf("Light - Started (%d)\n",chid);
    while (TRUE){
        change_light_t req = {.type = CHANGE_LIGHT_MSG_TYPE};
        change_light_resp_t res;
        MsgSend(chid, &req, sizeof(req), &res, sizeof(res));
        
        lockShmem(shmem);
        shmem->lightData.light = !shmem->lightData.light;
        printf("New Light value is %d\n", shmem->lightData.light);
        unlockShmem(shmem);
        
        sleep(5);
    }

    return EXIT_SUCCESS;
}
