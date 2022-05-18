/*
 * expAvg.h
 *
 *  Created on: May 16, 2022
 *      Author: Anil
 */


#ifndef _EXPAVG_H
#define _EXPAVG_H
#include "stdint.h"
//y(k) = kf * y(k-1) +  (1-kf) * x(k)
typedef struct{
	uint8_t input;
	uint8_t output;
	double kf;
}typExpavg_t;


inline void initExp(typExpavg_t *exp,double kf){
	exp->kf=kf;
	exp->output=0;
}

inline uint8_t iterateExp(typExpavg_t *exp,uint8_t input){
	exp->input= input;
	return exp->output = exp->kf * exp->output + exp->input * (1 - exp->kf);
}

#endif
