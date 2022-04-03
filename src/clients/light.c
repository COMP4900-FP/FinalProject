#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include <unistd.h>
#include "clients.h"

int loop_lights(int, void*);

int loop_lights(int chid, void* data){
    shared_data_t* shmem = (shared_data_t*) data;
    printf("Light - Started (%d)\n",chid);
    while (TRUE){
        lockShmem(shmem);
        change_light_t req = {
            .type = CHANGE_LIGHT_MSG_TYPE,
            .light_status = shmem->lightData.light
        };

        change_light_resp_t res;
        
        printf("Light: Got current light setting of %s using previous light setting of %s\n",BOOL_TO_LIGHT(req.light_status), BOOL_TO_LIGHT(req.light_status));

        MsgSend(chid, &req, sizeof(req), &res, sizeof(res));
        
        shmem->lightData.light = res.updated_light_status;
        printf("Light: Got new Light value of %s\n\n", BOOL_TO_LIGHT(res.updated_light_status));
        unlockShmem(shmem);
        
        sleep(5);
    }

    return EXIT_SUCCESS;
}
