/*
 * bluetooth.c
 *
 *  Created on: May 16, 2022
 *      Author: Anil
 */



#include "bluetooth.h"
#include "stm32f0xx.h"
#include "main.h"


extern UART_HandleTypeDef huart1;

uint8_t bltSendData(uint8_t buff){

	if(HAL_UART_Transmit(&huart1, &buff, sizeof(buff), 500)!=0){
		return 1;
	}
	return 0;
}
