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
    int loopCount = 0;
    lockShmem(shmem);
    uint8_t humidity = shmem->tempData.temp;
    unlockShmem(shmem);

    while (1){
        loopCount++;
        lockShmem(shmem);
        int curTemp = shmem->tempData.temp;
        MIN_BOUND(curTemp, 15);
        int offset = ((curTemp-15) / 2) - 4;
        //In +3/-4 range op so
        int ticksToDecrement = 12 - offset;
        if (loopCount % ticksToDecrement == 0){
            humidity++;
            if (humidity >= 90){
                shmem->soilData.saturation+= (humidity - 85) * 2;
                MAX_BOUND(shmem->soilData.saturation, 95);
                humidity = 80;
                printf("Humidity: Got high! Some condensate fell into the soil. Soil saturation now at %d%%", shmem->soilData.saturation);
            }
             shmem->humidityData.humidity = humidity;
            check_humidity_t req = {
                .type = CHECK_HUMIDITY_MSG_TYPE,
                .humidity_level = humidity};
            printf("Humidity: Got current humidity of %d%% using current temp of %d\n",req.humidity_level, shmem->tempData.temp);

            check_humidity_resp_t res;
            MsgSend(chid, &req, sizeof(req), &res, sizeof(res));
            if (res.updated_humidity_level != INVALID && res.updated_humidity_level != shmem->humidityData.humidity){
                shmem->humidityData.humidity = res.updated_humidity_level;
                printf("Humidity: Setting target humidity to %d%%\n\n",res.updated_humidity_level);
            }
            
        } 
        
        unlockShmem(shmem);
        usleep(100*1000);
    }



    return EXIT_SUCCESS;
}   
