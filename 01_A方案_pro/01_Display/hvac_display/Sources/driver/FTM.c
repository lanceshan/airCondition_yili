/*
 * FTM.c
 *
 *  Created on: Sep 15, 2015
 *      Author: Hill
 */
#include "FTM.h"

uint16 period[4] = {0, 0, 0, 0};
pt2FuncU8 FTM_Callback;

void FTM_Init()
{
	SIM_SCGC |= SIM_SCGC_FTM2_MASK; /* Enable Clock for FTM2 */
	
	FTM2_SC |= FTM_SC_PS(7);	/* Select Preescaler in this case 128. 20 Mhz /128 =156.25 Khz. */ 
								/* Counter increase by one every 6.4 us */
	/* Enable Channle 0*/
	FTM2_C0SC |= FTM_CnSC_CHIE_MASK; /* Enable channel 0 interrupt */
	FTM2_C0SC |= FTM_CnSC_MSA_MASK;  /* Channel as Output compare mode */
	
	/*Select interrupt frequency*/
	FTM2_C0V = FTM_CnV_VAL(period[0]) ;	 

	FTM2_SC |= FTM_SC_CLKS(1); /*FTM2 use system clock*/			 
}

/***********************************************************************************************
*
* @brief    FTM_SetPeriod - Set the period of the periodic interrupt
* @param    ch - channel to configure, per - period to set
* @return   none
*
************************************************************************************************/
void FTM_SetPeriod(uint8 ch, uint32 per)
{
	period[ch] = per;
	
	FTM2_C0V = FTM_CnV_VAL(period[0]) ;
	
}

/***********************************************************************************************
*
* @brief    FTM_SetCallback - Set the callback function to call on successful matches from any channel 
* @param    pointer to function with an UINT8 argument
* @return   none
*
************************************************************************************************/  
void FTM_SetCallback(pt2FuncU8 ptr)
{
	FTM_Callback = ptr;	
}

/***********************************************************************************************
*
* @brief    FTM2_IRQHandler - Interrupt for FTM channels, clear flag, set new tiemout and execute callback 
* @param    none
* @return   none
*
************************************************************************************************/
void FTM2_IRQHandler()
{	
	if (1==((FTM2_C0SC & FTM_CnSC_CHF_MASK)>>FTM_CnSC_CHF_SHIFT) )  /* If the CHF of the channel is equal to 0 */
	{
		(void)FTM2_C0SC;  							/* Read to clear flag */
		FTM2_C0SC ^= FTM_CnSC_CHF_MASK;  			/* Clear flag */
		FTM2_C0V = FTM2_C0V + FTM_CnV_VAL(period[0]) ; /* Refresh interrupt period */
		FTM_Callback();
	}	
}

/***********************************************************************************************
*
* @brief    Enable_Interrupt(UINT8 vector_number). Enable interrupts from desired module.
* @param    Module interrupt number from the interrupts vector table
* @return   none
*
************************************************************************************************/  
void Enable_Interrupt(uint8 vector_number)
{
	
	vector_number= vector_number-16;

	/* Set the ICPR and ISER registers accordingly */
	NVIC_ICPR |= 1 << (vector_number%32);
	NVIC_ISER |= 1 << (vector_number%32);

}
