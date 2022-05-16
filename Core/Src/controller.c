/*
 * controller.c
 *
 *  Created on: May 13, 2022
 *      Author: Anil
 */


void hoverInit(typHoverHandler *hhov){

	hhov->status=0;
	hhov->motorA->motorCode = 'A';
	hhov->motorB->motorCode = 'B';
	hhov->motorC->motorCode = 'C';
	hhov->motorD->motorCode = 'D';
}
uint8_t editBuffer(typeHoverHandler *hhov, typGyroHandler *hgyro, char buffer){

	buffer="";

	if(hhov->motorA->speed != hgyro->gyroA){

		hhov->motorA->speed = hgyro->gyroA;
		//EDIT BUFFER

	}
	if(hhov->motorB->speed != hgyro->gyroB){

		hhov->motorB->speed = hgyro->gyroB;
		//EDIT BUFFER

	}
	if(hhov->motorC->speed != hgyro->gyroC){

		hhov->motorC->speed = hgyro->gyroC;
		//EDIT BUFFER

	}
	if(hhov->motorD->speed != hgyro->gyroD){

		hhov->motorD->speed = hgyro->gyroD;
		//EDIT BUFFER

	}
}
