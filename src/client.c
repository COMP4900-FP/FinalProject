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

int main(void) {
	pid_t f1, f2, f3;
	int stat;
	if ((f1 = fork()) == 0){
		if ((f2 = fork()) == 0){
			puts("Component A");
			return EXIT_SUCCESS;

		} else {
			puts("Component B");
			waitpid(f2, &stat, 0);
		}
		return EXIT_SUCCESS;
	} else {
		if ((f3 = fork()) == 0){
			puts("Component C");
			return EXIT_SUCCESS;

		} else {
			puts("Component D");

			waitpid(f3, &stat, 0);
		}
		waitpid(f1, &stat, 0);
	}
	puts("Done!");
	return EXIT_SUCCESS;
}
