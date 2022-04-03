#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include "generatorFunctions.h"
#include "clients.h"

int *loop_temp(int, void*);

int *loop_temp(int chid, void* data){
    shared_data_t* shmem = (shared_data_t*) data;
    printf("Temp - Started (%d)\n", chid);
    int loopCount = 0;
    lockShmem(shmem);
    uint8_t temp = shmem->tempData.temp;
    uint8_t lastLight = shmem->lightData.light;

    unlockShmem(shmem);

    while (1){
        loopCount++;
        lockShmem(shmem);
        //Once every 2.5 hours
        if (loopCount % 50 == 0 && shmem->lightData.light == TRUE){
            if(shmem->lightData.light == lastLight)temp +=1;
        } else if (loopCount % 40 == 0 && shmem->lightData.light == FALSE){
            if (shmem->lightData.light == lastLight) temp -=1;
        } else {
            goto end;
        }

        MIN_BOUND(temp, 0);
        shmem->tempData.temp = temp;
        check_temperature_t req = {
            .type = CHECK_TEMP_MSG_TYPE,
			.light = shmem->lightData.light,
            .temp = temp};
        printf("Temp: Got current temp of %d using current light setting of %s\n",req.temp, BOOL_TO_LIGHT(shmem->lightData.light));
        check_temperature_resp_t res;
        MsgSend(chid, &req, sizeof(req), &res, sizeof(res));
        if (res.updated_temp != INVALID && shmem->tempData.temp != res.updated_temp){
            shmem->tempData.temp = res.updated_temp;
            temp = res.updated_temp;
            printf("Temp: Got new target value of %d\n\n", res.updated_temp);
        }

        end:
        unlockShmem(shmem);
        usleep(1000*100);
    }
    return EXIT_SUCCESS;

}
