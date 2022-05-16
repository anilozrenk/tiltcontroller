/*
 * controller.h
 *
 *  Created on: May 13, 2022
 *      Author: Anil
 */
#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stdint.h>

#define OK 'F'

typedef struct{
	char motorCode;
	uint8_t speed;

}typMotorHandler;
typedef struct{
	uint8_t status;
	typMotorHandler motorA;
	typMotorHandler motorB;
	typMotorHandler motorC;
	typMotorHandler motorD;
}typHoverHandler;
typedef struct{	  //0-255
	uint8_t gyroA;
	uint8_t gyroB;
	uint8_t gyroC;
	uint8_t gyroD;
}typGyroHandler;


void hoverInit(typHoverHandler *hhov);

uint8_t editBuffer(typeHoverHandler *hhov,
		typGyroHandler *hgyro, char buffer);






#endif





