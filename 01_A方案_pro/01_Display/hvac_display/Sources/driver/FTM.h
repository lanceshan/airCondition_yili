/*
 * FTM.h
 *
 *  Created on: Sep 15, 2015
 *      Author: Hill
 */

#ifndef FTM_H_
#define FTM_H_

#include "derivative.h"

void FTM_Init(void);

void FTM_SetCallback(pt2FuncU8 ptr);

void FTM_SetPeriod(uint8 ch, uint32 per);

void FTM2_IRQHandler(void);

void Enable_Interrupt(uint8 vector_number);

#endif /* TIMER_H_ */
