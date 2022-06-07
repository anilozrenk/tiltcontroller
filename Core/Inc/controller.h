/*
 *  @file controller.h
 *  @author: Anil Ozrenk
 *  @date 05/13/2022
 *
 */
#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stdint.h>

#define OK 'F' ///< ascii character for OK

#define MAX_POWER 255
///enum for states of moving states
enum vector_state{
	forward,
	turn_left,
	turn_right,
	forward_left,
	forward_right,
	idle,
	halt,

};
/// individual motor
typedef struct{
	char motorCode; ///< ascii code for a motor
	uint8_t speed; ///< 0-255

}typMotorHandler;
/// struct for hovercraft
typedef struct{
	uint8_t status;
	typMotorHandler motorA; ///< motor left
	typMotorHandler motorB; ///< motor right
	typMotorHandler motorC; ///< motor down
	typMotorHandler motorD; ///< motor down
}typHoverHandler;
/// unprossessed pwms for motors
typedef struct{
	uint8_t pwmInputA;
	uint8_t pwmInputB;
	uint8_t pwmInputC;
	uint8_t pwmInputD;
}typPWMInputHandler;
/**
 * OBSOLETE
 * output pwm for motors
 */
/// obselete
typedef struct{	  //0-255
	uint8_t pwmOutputA;
	uint8_t pwmOutputB;
	uint8_t pwmOutputC;
	uint8_t pwmOutputD;
}typPWMOutputHandler;
/// vector indentifier
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
void pwmSmooting(typHoverHandler *hHov,typPWMInputHandler *input,double kf);
void deathzonefit(double *delta_x,double pmax,double nmax,double death_zone);
void command(typHoverHandler *hHov,char *buff);





#endif





