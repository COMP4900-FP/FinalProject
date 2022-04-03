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

extern int loop_lights(int);
extern int loop_temp(int);
extern int loop_humidity(int);
extern int loop_soil(int);


int startRunner(int(fun)(int)){
	//Eventually can replace with a ChannelCreate call
	int chid = name_open(GREENHOUSE_SERVER_NAME, 0);
	return fun(chid);
}

int main(void) {
	pid_t f1, f2, f3;
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
			return startRunner(loop_humidity);
		} else {
			startRunner(loop_soil);
			waitpid(f3, &stat, 0);
		}
		waitpid(f1, &stat, 0);
	}
	return EXIT_SUCCESS;
}
