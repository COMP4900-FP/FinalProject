/*
 * generatorFunctions.h
 *
 *  Created on: Mar. 28, 2022
 *      Author: Hayden
 */

#ifndef SRC_GENERATORFUNCTIONS_H_
#define SRC_GENERATORFUNCTIONS_H_
#define SPRINKLER_TIME 30 //Defines the period by which the sprinklers turn on/off
#define LIGHT_TIME 30//Defines the period by which the lights turn on/off

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>


uint8_t getHumidity(uint8_t, uint8_t);
uint8_t getTemp(uint8_t);
uint8_t getSoilMoisture(uint8_t);

#endif /* SRC_GENERATORFUNCTIONS_H_ */
