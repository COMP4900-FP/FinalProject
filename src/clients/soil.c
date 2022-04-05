#include <stdlib.h>
#include <stdio.h>
#include "server.h"
#include "generatorFunctions.h"
#include <unistd.h>
#include "clients.h"
int loop_soil(int, void*);

int loop_soil(int chid, void* data){
    shared_data_t* shmem = (shared_data_t*) data;
    printf("Soil - Started \n");
    int loopCount = 0;
    lockShmem(shmem);
    uint8_t soil = shmem->soilData.saturation;
    unlockShmem(shmem);

    while (1){
        loopCount ++;
        lockShmem(shmem);
        //Higher temps  -> Higher number
        int curTemp = shmem->tempData.temp;
        MIN_BOUND(curTemp, 15);
        int offset = ((curTemp-15) / 3) - 2;
        
        //In 8-13 range op so
        int ticksToDecrement = 10 - offset;

        if (loopCount % ticksToDecrement == 0){
           // printf("Soil: Currently decrementing every %d ticks\n", ticksToDecrement);
            soil--;
            MIN_BOUND(soil, 1);
            shmem->soilData.saturation = soil;

            distribute_water_t req = {
                    .type = DISTRIBUTE_WATER_MSG_TYPE,
                    .saturation = soil};
            printf("SOIL: Current saturation: %d, using temp of %d\n",req.saturation, shmem->tempData.temp);

            distribute_water_resp_t res;
            MsgSend(chid, &req, sizeof(req), &res, sizeof(res));
            if (res.saturation != INVALID && shmem->soilData.saturation != res.saturation){
                shmem->soilData.saturation = res.saturation;
                soil = res.saturation;
                printf("SOIL: New value: %d\n\n", res.saturation);
            }
        }
        unlockShmem(shmem);
        usleep(1000*100);
    }


    return EXIT_SUCCESS;
}

