/*
 * generatorFunctions.c
 *
 *  Created on: Mar. 28, 2022
 *      Author: Hayden
 */
#include "generatorFunctions.h"

//Returns a humidity value between 1 and 10
float getHumidity(){
	int randRange = 9;
	float humidity = 0.00;
	srand((int)time(NULL));
	humidity = (float)rand()/(float)(RAND_MAX/randRange) + 1.00;

	return humidity;
}

//Returns a temperature value between 0 and 30
//Assume the values are Celsius
float getTemp(){
	int randRange = 30;
	float temp = 0.00;
	srand((int)time(NULL));
	temp = (float)rand()/(float)(RAND_MAX/randRange);

	return temp;
}
