/*
 * valve.c
 *
 *  Created on: Jul 30, 2015
 *      Author: Hill
 */
#include "valve.h"
#include "Valve_Ctrl.h"

void Open_Valve(void)
{
	Valve_Ctrl_SetVal();
}

void Close_Valve(void)
{
	Valve_Ctrl_ClrVal();
}

