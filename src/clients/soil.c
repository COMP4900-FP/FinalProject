#include <stdlib.h>
#include <stdio.h>
#include "server.h"
#include "generatorFunctions.h"
#include <unistd.h>

int loop_soil(int, void*);

int loop_soil(int chid, void* data){
    shared_data_t* shmem = (shared_data_t*) data;
    printf("Soil - Started (%d)\n", chid);
    while (1){
        lockShmem(shmem);
        distribute_water_t req = {
                .type = DISTRIBUTE_WATER_MSG_TYPE,
                .saturation = getSoilMoisture(shmem->tempData.targetTemp)};
        printf("Soil: Got current saturation of %d using?\n",req.saturation);

        distribute_water_resp_t res;
        MsgSend(chid, &req, sizeof(req), &res, sizeof(res));
        shmem->soilData.targetSaturation = res.targetSaturation;
        printf("Soil: Got new target value of %d\n\n", res.targetSaturation);
        unlockShmem(shmem);
        usleep(1000*900);
    }


    return EXIT_SUCCESS;
}

