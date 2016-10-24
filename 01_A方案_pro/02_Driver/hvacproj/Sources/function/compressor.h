/*
 * compressor.h
 *
 *  Created on: Jul 30, 2015
 *      Author: Hill
 */

#ifndef COMPRESSOR_H_
#define COMPRESSOR_H_

#include "driver\gpio.h"
/**********压缩机机运行档位************/
#define CmpFrq_Lv0 0
#define CmpFrq_Lv1 30
#define CmpFrq_Lv2 38
#define CmpFrq_Lv3 46
#define CmpFrq_Lv4 54
#define CmpFrq_Lv5 62
#define CmpFrq_Lv6 70
#define CmpFrq_Lv7 78
#define CmpFrq_Lv8 85

#define CMP_SWITCH_TIME 	100		//压缩机关闭后频率逐渐降低时间间隔 100*10ms

void Close_Cmp(unsigned char *, unsigned int *);	
void Set_CmpLv(unsigned char cmpLv);
void Set_Cmp(unsigned char cmpLv);

#endif /* COMPRESSOR_H_ */
