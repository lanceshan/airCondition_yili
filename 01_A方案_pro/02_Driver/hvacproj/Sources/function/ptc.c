/*
 * ptc.c
 *
 *  Created on: Jul 30, 2015
 *      Author: Hill
 */
#include "ptc.h"

void Set_PTC(unsigned char num)
{
	switch (num)
	{
		case 0:
			PTC_Ctrl_ClrVal();
			break;
		case 1:
			PTC_Ctrl_SetVal();			
			break;
		default:
			break;
	}
}
