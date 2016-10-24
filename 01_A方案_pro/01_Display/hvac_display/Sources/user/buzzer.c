/*
 * buzzer.c
 *
 *  Created on: Apr 28, 2016
 *      Author: Hill
 */

#include "buzzer.h"

void Buzzer(void)
{
	unsigned int i = 100;
		
	for(i=100; i>0; i--)
		Set_Buzzer;
	
	Set_Buzzer_L;
}

