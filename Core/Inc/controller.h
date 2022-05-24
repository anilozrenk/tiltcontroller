/*
 * controller.h
 *
 *  Created on: May 13, 2022
 *      Author: Anil
 */
#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stdint.h>

#define OK "F"
#define MAX_POWER 255

enum vector_state{
	forward,
	turn_left,
	turn_right,
	forward_left,
	forward_right,
	idle,
	halt,

};
typedef struct{
	char motorCode;
	uint8_t speed;

}typMotorHandler;
typedef struct{
	uint8_t status;
	typMotorHandler motorA;//sol
	typMotorHandler motorB;//sag
	typMotorHandler motorC;//arka
	typMotorHandler motorD;//on
}typHoverHandler;
typedef struct{	  //0-255
	uint8_t pwmInputA;
	uint8_t pwmInputB;
	uint8_t pwmInputC;
	uint8_t pwmInputD;
}typPWMInputHandler;
typedef struct{	  //0-255
	uint8_t pwmOutputA;
	uint8_t pwmOutputB;
	uint8_t pwmOutputC;
	uint8_t pwmOutputD;
}typPWMOutputHandler;
typedef struct{
	uint8_t forward;
	uint8_t left;
	uint8_t right;
	uint8_t backward;
}typVector;

void vectorToPwm(typVector *hVec, typPWMInputHandler *pwmInput);
void angleToVector(typVector *hVec,double curr_angle_x,double start_angle_x,double curr_angle_y,double start_angle_y ,double death_zone);
void deathzone(double *delta_x,double pmax,double nmax,double death_zone);
void hoverInit(typHoverHandler *hhov);
//uint8_t vectorState(typVector *vector);
void pwmSmooting(typHoverHandler *hHov,typPWMInputHandler *input,double kf);
void deathzonefit(double *delta_x,double pmax,double nmax,double death_zone);
void command(typHoverHandler *hHov,char *buff);
//void pwmToAscii(typPWMOutputHandler *pwmout);


//uint8_t editBuffer((typHoverHandler *hhov, typPWMOutputHandler *pwmOut, char *buffer));






#endif





