/*
 * timer.c
 *
 *  Created on: Sep 17, 2015
 *      Author: Hill
 */
#include "timer.h"
#include "globalvars.h"
#include "gpio.h"
#include "display.h"

#define LED_TOGGLE	OUTPUT_TOGGLE(A,PTA1)
#define LED_OFF		OUTPUT_CLEAR(A,PTA1)
#define LED_ON		OUTPUT_SET(A,PTA1)

void Timer_Interrupt(void)
{
	LED_TOGGLE;
	if(gSetOKTime > 0)gSetOKTime--;
	if(gBlinkTime < (2 * BLINK_FRQ_TIME))gBlinkTime++;	
	if(gSkimTime > 0)gSkimTime--;
	if(gSkimTime == 0)gSkimFlag = 0;
	if(gKeyPresTime < 100)gKeyPresTime++;
	if(gCANRxFailTime < 100)gCANRxFailTime++;
	if((gKeyPresTime % KEY_NEXT_CHECK_TIME) == 0)gKeyHandleFlag = 1;
	if(gKeyPresTime >= KEY_HOLD_TIME)
	{
		gKeyPresHold.Flags = (uint8)(1 << (gKeyStatus - 1));
	}
	else
	{
		gKeyPresHold.Flags = 0;
	}
	if((gSys_Info.Errorflags.Byte != 0) || (gSys_Info.ErrTempers.Byte != 0) || (gSys_Info.ErrPower.Byte != 0) || (gSkimFlag == 1))	
	{
		if(gUser_Info->Power == POWER_ON)		//����״̬�£�ϵͳ���ϱ����Ͳ�ѯʱ���̶�Ƶ��ˢ����ʾ���棬�Դﵽͼ����˸Ч��
			Handle_DisBlinkBmp(gHT1621_RAM);	//�����������ԣ��ж���ʹ���Ǻ����������ط�����
	}
}

void Timer_Init(void)
{
	FTM_Init();							/* Initialize the FTM module */
	FTM_SetPeriod(0, 15625);			/* Interrupt every 15625 counts, equal to every 100 ms */					
	FTM_SetCallback(Timer_Interrupt); 	/* Set the callback function to be called */
	Enable_Interrupt(INT_FTM2);  		/* Enable FTM2 interrupt */
	
}

