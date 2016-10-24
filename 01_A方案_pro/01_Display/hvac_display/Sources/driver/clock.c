/*
 * clock.c
 *
 *  Created on: Sep 15, 2015
 *      Author: Hill
 */
#include"clock.h"

void Clk_Init(void)
{
	ICS_C1|=ICS_C1_IRCLKEN_MASK; 		/* Enable the internal reference clock*/ 
	ICS_C3= 0x90;						/* Reference clock frequency = 31.25 KHz*/ //???如何选择了31.25kHz		
	while(!(ICS_S & ICS_S_LOCK_MASK));  /* Wait for PLL lock, now running at 40 MHz (1280 * 31.25Khz) */		
    ICS_C2|=ICS_C2_BDIV(1); 			/* BDIV=2, Bus clock = 20 MHz*/
	ICS_S |= ICS_S_LOCK_MASK; 			/* Clear Loss of lock sticky bit */	
}
