/*
 * ht1621.h
 *
 *  Created on: Apr 19, 2016
 *      Author: Hill
 */

#ifndef HT1621_H_
#define HT1621_H_

#include "driver\gpio.h"

#define HT1621_Set_CS_OUT		CONFIG_PIN_AS_GPIO(A,PTB0,OUTPUT)
#define HT1621_Set_CS_H			OUTPUT_SET(A,PTB0)
#define HT1621_Set_CS_L			OUTPUT_CLEAR(A,PTB0)

#define HT1621_Set_RD_OUT		CONFIG_PIN_AS_GPIO(B,PTF3,OUTPUT)
#define HT1621_Set_RD_H			OUTPUT_SET(B,PTF3)
#define HT1621_Set_RD_L			OUTPUT_CLEAR(B,PTF3)

#define HT1621_Set_WR_OUT		CONFIG_PIN_AS_GPIO(B,PTF2,OUTPUT)
#define HT1621_Set_WR_H			OUTPUT_SET(B,PTF2)
#define HT1621_Set_WR_L			OUTPUT_CLEAR(B,PTF2)

#define HT1621_Set_DA_H			OUTPUT_SET(A,PTA7)
#define HT1621_Set_DA_L			OUTPUT_CLEAR(A,PTA7)
#define HT1621_Get_DA			INPUT_GET(A,PTA7)
#define HT1621_Set_DA_IN		CONFIG_PIN_AS_GPIO(A,PTA7,INPUT)
#define HT1621_EN_DA_IN			ENABLE_INPUT(A,PTA7)
#define HT1621_Set_DA_OUT		CONFIG_PIN_AS_GPIO(A,PTA7,OUTPUT)  

void Ht1621_Init(void);
void HT1621_RAM_Update(unsigned char buf[][13]);
#endif /* HT1621_H_ */
