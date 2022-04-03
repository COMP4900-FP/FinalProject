/*
 * generatorFunctions.c
 *
 *  Created on: Mar. 28, 2022
 *      Author: Hayden
 */
#include "generatorFunctions.h"

//Returns a humidity value between 1 and 100
//Assume the values are standard humidity measurement units (in percentages)
float getHumidity(){
	int randRange = 99;
	float humidity = 0.00;
	srand((int)time(NULL));
	humidity = (float)rand()/(float)(RAND_MAX/randRange) + 1.00;

	return humidity;
}

//Returns a temperature value between 0 and 30
//Assume the values are Celsius
float getTemp(uint8_t areLightsOn){
	int randRange = 15;
	float temp = 0.00;
	temp = (float)rand()/(float)(RAND_MAX/randRange);
	temp += areLightsOn?15:5;
	return temp;
}
