#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include "generatorFunctions.h"
#include "clients.h"

int *loop_temp(int, void*);

int *loop_temp(int chid, void* data){
    shared_data_t* shmem = (shared_data_t*) data;
    printf("Temp - Started (%d)\n", chid);
    while (1){
        lockShmem(shmem);
        check_temperature_t req = {
            .type = CHECK_TEMP_MSG_TYPE,
            .temp = getTemp(shmem->lightData.light),
			.light = shmem->lightData.light};
        printf("Temp: Got current temp of %d using current light setting of %s\n",req.temp, BOOL_TO_LIGHT(shmem->lightData.light));

        check_temperature_resp_t res;
        MsgSend(chid, &req, sizeof(req), &res, sizeof(res));
        shmem->tempData.targetTemp = res.updated_temp;
        printf("Temp: Got new target value of %d\n\n", res.updated_temp);

        unlockShmem(shmem);
        usleep(1000*900);
    }
    return EXIT_SUCCESS;

}
