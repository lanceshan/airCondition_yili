/*
 * adc.c
 *
 *  Created on: Jul 30, 2015
 *      Author: Hill
 */
#include "cpu.h"
#include "SKEAZ1284.h"
#include "adc.h"

void adc_init(void)
{
	SIM_SCGC |= SIM_SCGC_ADC_MASK;
	ADC_SC1 = 0	;	
	ADC_SC3 = 0x29;  // 0b0010 1001, adiv = 2, mode = 12bit, adiclk = bus/2
	ADC_SC2 = 0x00;
	ADC_APCTL1 |= 0x00000001<<0x0f;
	ADC_APCTL1 |= 0x00000001<<0x0e;
	ADC_APCTL1 |= 0x00000001<<0x0d;
	ADC_APCTL1 |= 0x00000001<<0x0c;
	ADC_APCTL1 |= 0x00000001<<0x0b;
	ADC_APCTL1 |= 0x00000001<<0x0a;
	ADC_APCTL1 |= 0x00000001<<0x09;
	ADC_APCTL1 |= 0x00000001<<0x08;
	ADC_APCTL1 |= 0x00000001<<0x07;
	ADC_APCTL1 |= 0x00000001<<0x06;
}

uint16 adc_rd(unsigned char port)
{
	const unsigned char adch[] = {0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x16};
	ADC_SC1 = adch[port];
	while(!(ADC_SC1 & 0x80));
	return ADC_R & 0xffffffff;
}
/*
typedef  enum{
	DCVoltage=0,   	//ch6   PTB2
	ReserTemper,	//ch7	PTB3
	InPipeTemper,	//ch8	PTC0
	OutPipeTemper,	//ch9	PTC1
	LeftTemper,		//ch10	PTC2
	RightTemper,	//ch11	PTC3
	CmpTemper,		//ch12	PTF4
	StoreTemper,	//ch13	PTF5
	AirTemper,		//ch14	PTF6
	OutTemper, 		//ch15	PTF7
	MCUTemper		//ch22	IN
}ADC_CHANNEL;
*/
