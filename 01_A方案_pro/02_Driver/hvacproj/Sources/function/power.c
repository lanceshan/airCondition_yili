/*
 * power.c
 *
 *  Created on: Sep 11, 2015
 *      Author: Hill
 */
#include "power.h"
#include "Master_Ctrl.h"

void Power_ON(void)
{
	Master_Ctrl_SetVal();
}

void Power_OFF(void)
{
	Master_Ctrl_ClrVal();
}
