/*
 * generatorFunctions.c
 *
 *  Created on: Mar. 28, 2022
 *      Author: Hayden
 */
#include "generatorFunctions.h"

//Returns a humidity value between 1 and 100
//Assume the values are standard humidity measurement units (in percentages)
uint8_t getHumidity(uint8_t currentTemp, uint8_t soilMoisture){
	
	float humidity = (float)rand()/(float)(RAND_MAX/10) + 20;
	humidity += (float) soilMoisture / (float)7;
	humidity += (currentTemp - 20);

	return humidity;
}

//Returns a temperature value between 0 and 30
//Assume the values are Celsius
uint8_t getTemp(uint8_t areLightsOn){
	int randRange = 15;
	float temp = 0.00;
	temp = (float)rand()/(float)(RAND_MAX/randRange);
	temp += areLightsOn?15:5;
	return temp;
}

uint8_t getSoilMoisture(uint8_t temp){
	float soil = 0.00;
	soil = (float)rand()/(float)(RAND_MAX/10);
	soil += 36;
	if (temp > 22){
		soil -= (temp-22);
	} 
	return soil;
}
