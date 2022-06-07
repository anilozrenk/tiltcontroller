/**
 * @file controller.c
 * @brief all process angle data to sendable buffer
 * @date 05/13/2022
 * @author Anil
 */

#include "controller.h"

/*  Local Functions  */
void pwmToAscii(typMotorHandler *hmotor,char *buff);
void deathzonefit(double *delta_x,double pmax,double nmax,double death_zone);
uint8_t vectorState(typVector *vector);
/********    ********/
/**
 * @brief intiate hovercraft
 * give motors ascii characters
 * set all motors speeds to 0
 * @param typHoverHandler
 */
void hoverInit(typHoverHandler *hhov){

	hhov->status=0;
	hhov->motorA.motorCode = 'A';
	hhov->motorB.motorCode = 'B';
	hhov->motorC.motorCode = 'C';
	hhov->motorD.motorCode = 'D';
	hhov->motorA.speed=0;
	hhov->motorB.speed=0;
	hhov->motorC.speed=0;
	hhov->motorD.speed=0;
}

/**
 * @brief exponantiel filter for smoothing motor pwm
 * @param typHoverHandler
 * @param typPWMInputHandler
 */
void pwmSmooting(typHoverHandler *hHov,typPWMInputHandler *input,double kf){
	hHov->motorA.speed = (1-kf) * hHov->motorA.speed + input->pwmInputA *kf;
	hHov->motorB.speed = (1-kf) * hHov->motorB.speed + input->pwmInputB *kf;
	hHov->motorC.speed = (1-kf) * hHov->motorC.speed + input->pwmInputC *kf;
	hHov->motorD.speed = (1-kf) * hHov->motorD.speed + input->pwmInputD *kf;

}

/**
 * @brief decide the state of vectors
 * @param typVector
 */
uint8_t vectorState(typVector *vector){
	if((vector->forward==0)&&(vector->left==0)&&(vector->right==0)&&(vector->backward==0)){
		return idle;
	}
	if(vector->backward>15){
		return halt;
	}
	if((vector->forward>0)&&(vector->left==0)&&(vector->right==0)&&(vector->backward==0)){
			return forward;
		}
	if((vector->left>0)&&(vector->forward==0)&&(vector->right==0)&&(vector->backward==0)){
		return turn_left;
	}
	if((vector->right>0)&&(vector->forward==0)&&(vector->left==0)&&(vector->backward==0)){
		return turn_right;
	}
	if((vector->left>0)&&(vector->forward>0)&&(vector->right==0)&&(vector->backward==0)){
		return forward_left;
	}
	if((vector->right>0)&&(vector->forward>0)&&(vector->left==0)&&(vector->backward==0)){
		return forward_right;
	}
	else return idle;

}
/**
 * @brief generate pwm value corrosponding to vector states
 * @param typPWMInputHandler
 * @param typVector
 */
void vectorToPwm(typVector *hVec, typPWMInputHandler *pwmInput){
	uint8_t state;
	state = vectorState(hVec);
	switch (state) {
		case forward:
			pwmInput->pwmInputA=hVec->forward;
			pwmInput->pwmInputB=hVec->forward;
			break;
		case turn_left:
			pwmInput->pwmInputB=hVec->left;
			break;
		case turn_right:
			pwmInput->pwmInputA=hVec->right;
			break;
		case forward_left:
			pwmInput->pwmInputA=(hVec->forward)/2;
			pwmInput->pwmInputB=(hVec->forward+hVec->left)/2;
			break;
		case forward_right:
			pwmInput->pwmInputB=(hVec->forward)/2;
			pwmInput->pwmInputA=(hVec->forward+hVec->right)/2;
			break;
		case halt:
			pwmInput->pwmInputA=0;
			pwmInput->pwmInputB=0;
			pwmInput->pwmInputC=0;
			pwmInput->pwmInputD=0;
			break;
		case idle:
			pwmInput->pwmInputC=120;
			pwmInput->pwmInputD=120;
			break;
		default:
			break;
	}
}
/**
 * @brief limit the angle and add deathzone
 * @param delta_angle
 * @param positive max
 * @param negative max
 * @param deathzone
 */
void deathzonefit(double *delta_x,double pmax,double nmax,double death_zone){
		if(*delta_x>pmax){
			 *delta_x=pmax;
		}
		if(*delta_x<nmax){
			*delta_x=nmax;
		}
		if ((*delta_x<death_zone)&&(*delta_x>0)) {
			*delta_x=0;
		}
		if ((*delta_x > (-1*death_zone))&&(*delta_x<0)) {
			*delta_x=0;
		}
	}
/**
 * @brief angle values to vector calues
 * @param typVector
 * @param angle_x
 * @param base_angle_x
 * @param angle_y
 * @param base_angle_y
 * @param deathzone
 */
void angleToVector(typVector *hVec,double curr_angle_x,double start_angle_x,double curr_angle_y,double start_angle_y ,double death_zone){
	 double pmax = 45;
	 double nmax = -45;
	 double delta_x = curr_angle_x - start_angle_x;
	 double delta_y = curr_angle_y - start_angle_y;
	 deathzonefit(&delta_x, pmax, nmax, death_zone);
	 deathzonefit(&delta_y, pmax, nmax, death_zone);
	 if(delta_y>=0){
		 if(delta_x<0){
			 hVec->left = delta_x*255/nmax;
			 hVec->right = 0;
		 }
		 else if(delta_x>0){
			 hVec->right = delta_x*255/pmax;
			 hVec->left = 0;
		 }
		 else{
			 hVec->right = 0;
			 hVec->left = 0;
		 }
		 hVec->forward= delta_y*255/pmax;
		 hVec->backward= 0;
	 }
	 else if(-30<delta_y&&delta_y<0){
		 hVec->backward=0;
		 hVec->left=0; hVec->right=0; hVec->forward=0;
	 }
	 else{
		 hVec->backward=50;
				 hVec->left=0; hVec->right=0; hVec->forward=0;
	 }

}
/*
 * @brief generate sendable buffer for hovercraft
 * @param typHoverHandler
 * @param char *buffer
 */
void command(typHoverHandler *hHov,char *buff){
	char buffA[3],buffB[3],buffC[3],buffD[3];
	pwmToAscii(&(hHov->motorA), buffA);
	pwmToAscii(&(hHov->motorB), buffB);
	pwmToAscii(&(hHov->motorC), buffC);
	pwmToAscii(&(hHov->motorD), buffD);
	uint8_t i=0;
	uint8_t j=0;
	while(i<12){
		while(j<3){
			buff[i]=buffA[j];
			i++;
			j++;
		}
		j=0;
		while(j<3){
			buff[i]=buffB[j];
			i++;
			j++;
		}
		j=0;
		while(j<3){
			buff[i]=buffC[j];
			i++;
			j++;
		}
		j=0;
		while(j<3){
			buff[i]=buffD[j];
			i++;
			j++;
		}
	}
	buff[12]='F';
}
/**
 * @brief 8 bit unsigned integer value to string
 * @param typMotorHandler
 * @param char *buffer
 */
void pwmToAscii(typMotorHandler *hmotor,char *buff){
	char highbytes =(char) hmotor->speed >> 4;
	char sendfirst = highbytes+97;
	char lowbytes = (char)hmotor->speed & 0x0F;
	char sendlast = lowbytes+97;
	buff[0]=hmotor->motorCode;
	buff[1]=sendfirst;
	buff[2]=sendlast;
}










