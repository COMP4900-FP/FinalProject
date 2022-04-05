#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include <unistd.h>
#include "clients.h"
#include <time.h>

int loop_lights(int, void*);

int loop_lights(int chid, void* data){
    shared_data_t* shmem = (shared_data_t*) data;
    printf("Light - Started (%d)\n",chid);
    while (TRUE){
        lockShmem(shmem);
        change_light_t req = {
            .type = CHANGE_LIGHT_MSG_TYPE,
            .light_status = shmem->lightData.light,
            //Simulate hour of the day every 2 secs.
            .hourOfDay = HOUR_OF_DAY
        };

        change_light_resp_t res;
        
        //printf("Light: Got current light setting of %s using previous light setting of %s\n",BOOL_TO_LIGHT(reqlightsActivated_status), BOOL_TO_LIGHT(reqlightsActivated_status));

        MsgSend(chid, &req, sizeof(req), &res, sizeof(res));
        if (res.updated_light_status != INVALID && res.updated_light_status != shmem->lightData.light){
            shmem->lightData.light = res.updated_light_status;
            printf("LIGHTt: New value: %s\n\n", BOOL_TO_LIGHT(res.updated_light_status));
        }

        unlockShmem(shmem);
        sleep(1);
    }

    return EXIT_SUCCESS;
}
