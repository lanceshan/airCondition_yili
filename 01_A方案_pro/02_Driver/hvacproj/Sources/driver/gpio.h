/*
 * gpio.h
 *
 *  Created on: Jul 30, 2015
 *      Author: Hill
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "VFD_ON_OFF.h"
#include "VFD_FM1.h"
#include "VFD_FM2.h"
#include "VFD_FM3.h"

#define MI1_ON		VFD_ON_OFF_SetVal()	
#define MI1_OFF		VFD_ON_OFF_ClrVal()
#define MI3_ON		VFD_FM1_SetVal()
#define MI3_OFF		VFD_FM1_ClrVal()
#define MI4_ON		VFD_FM2_SetVal()
#define MI4_OFF		VFD_FM2_ClrVal()
#define MI5_ON		VFD_FM3_SetVal()
#define MI5_OFF		VFD_FM3_ClrVal()

#endif /* GPIO_H_ */
