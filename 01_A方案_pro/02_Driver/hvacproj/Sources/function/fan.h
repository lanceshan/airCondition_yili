/*
 * fan.h
 *
 *  Created on: Jul 30, 2015
 *      Author: Hill
 */

#ifndef FAN_H_
#define FAN_H_

#include "OutsideFan_PWM.h"
#include "InnerFan_PWM.h"
#include "LeftFresh_CLK.h"
#include "LeftFresh_Dir.h"
#include "RightFresh_CLK.h"
#include "RightFresh_Dir.h"
#include "StoreFan_Ctrl.h"
#include "LeftFresh_Photo.h"
#include "RightFresh_Photo.h"
#include "LeftFresh_Enable.h"
/**********蒸发风机运行档位************/
#define Blower_Lv0 5
#define Blower_Lv1 12
#define Blower_Lv2 25
#define Blower_Lv3 35
#define Blower_Lv4 45
#define Blower_Lv5 55
#define Blower_Lv6 65
/**********冷凝风机运行档位************/
#define OUTSIDE_FAN_OFF    5
#define OUTSIDE_FAN_LOW 	60
#define OUTSIDE_FAN_HIGH 	80

void Set_OutsideFan_Speed(unsigned char freq);

void Set_InnerFan_Speed(unsigned char WindLv);

void Open_StoreFan(void);
void Close_StoreFan(void);

#endif /* FAN_H_ */
