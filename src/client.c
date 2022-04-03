/*
 * ClientLauncher.c
 *
 *  Created on: Mar 27, 2022
 *      Author: cbains
 */

#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include <unistd.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include "clients.h"
unsigned long start;

extern int loop_lights(int,void*);
extern int loop_temp(int,void*);
extern int loop_humidity(int,void*);
extern int loop_soil(int,void*);

shared_data_t* shared = NULL;


int startRunner(int(fun)(int, void*)){
	//Eventually can replace with a ChannelCreate call
	int chid = name_open(GREENHOUSE_SERVER_NAME, 0);
	return fun(chid, shared);
}

void initSHMem(){
	pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shared->dataMutex, &attr);
	shared->lightData.light = FALSE;
	shared->soilData.saturation = 50;
	shared->tempData.temp = 25;
	shared->humidityData.humidity = 90;
}

int main(void) {
	srand((int)time(NULL));
	start =  (unsigned long)time(NULL);
	shared = mmap(NULL, sizeof(shared_data_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	initSHMem();

	pid_t f1, f2, f3, f4;
	int stat;
	if ((f1 = fork()) == 0){
		if ((f2 = fork()) == 0){
			return startRunner(loop_lights);

		} else {
			startRunner(loop_temp);
			waitpid(f2, &stat, 0);
		}
		return EXIT_SUCCESS;
	} else {
		if ((f3 = fork()) == 0){
			if ((f4 = fork())==0){
				return startRunner(loop_humidity);
			} else {
				while (TRUE){
					if (HOUR_OF_DAY % 24 == 0){
						printf("A New day has dawned. 0:00\n\n");
						//Delay for at least a hour
						sleep(2);
					}
					//Delay for a half hour
					sleep(1);
				}
				waitpid(f4, &stat, 0);
			}
			
		} else {
			startRunner(loop_soil);
			waitpid(f3, &stat, 0);
		}
		waitpid(f1, &stat, 0);
	}
	return EXIT_SUCCESS;
}
