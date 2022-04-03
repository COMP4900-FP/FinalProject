#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include <unistd.h>
#include "generatorFunctions.h"
#include "clients.h"


int loop_humidity(int,void*);

int loop_humidity(int chid, void* data){
    shared_data_t* shmem = (shared_data_t*) data;
    printf("Humidity - Started (%d)\n", chid);

    while (1){
        lockShmem(shmem);
        check_humidity_t req = {
            .type = CHECK_HUMIDITY_MSG_TYPE,
            .humidity_level = getHumidity(shmem->tempData.targetTemp, shmem->soilData.targetSaturation)};
        printf("Humidity: Got current humidity of %d%% using current temp of %d and current soil moisture level of %d\n",req.humidity_level, shmem->tempData.targetTemp, shmem->soilData.targetSaturation);

        check_humidity_resp_t res;
        MsgSend(chid, &req, sizeof(req), &res, sizeof(res));
        shmem->humidityData.targetHumidity = res.updated_humidity_level;
        printf("Humidity: Setting target humidity to %d%%\n\n",res.updated_humidity_level);
        unlockShmem(shmem);
        sleep(10);
    }



    return EXIT_SUCCESS;
}   
