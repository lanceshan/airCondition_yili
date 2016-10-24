/*
 * gpio.h
 *
 *  Created on: Sep 15, 2015
 *      Author: Hill
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "derivative.h"

void GPIO_Init(void);

#define Set_Buzzer_H		OUTPUT_SET(A,PTB1)
#define Set_Buzzer_L		OUTPUT_CLEAR(A,PTB1)
#define Set_Buzzer			OUTPUT_TOGGLE(A,PTB1)

#define Get_KEY_POWER		INPUT_GET(A,PTB3)
#define Get_KEY_MODE		INPUT_GET(A,PTD1)
#define Get_KEY_AUTO		INPUT_GET(B,PTF6)
#define Get_KEY_PTC			INPUT_GET(A,PTD0)
#define Get_KEY_FRESHFAN	INPUT_GET(B,PTF4)
#define Get_KEY_SKIM		INPUT_GET(A,PTA2)
#define Get_KEY_TEMP_PLUS	INPUT_GET(B,PTH7)
#define Get_KEY_TEMP_MINUS	INPUT_GET(A,PTA3)
#define Get_KEY_WIND_PLUS	INPUT_GET(B,PTF5)
#define Get_KEY_WIND_MINUS	INPUT_GET(A,PTB2)

enum GPIOA {
PTA0,	
PTA1,
PTA2,
PTA3,
PTA4,
PTA5,
PTA6,
PTA7,
PTB0,	
PTB1,
PTB2,
PTB3,
PTB4,
PTB5,
PTB6,
PTB7,
PTC0,	
PTC1,
PTC2,
PTC3,
PTC4,
PTC5,
PTC6,
PTC7,
PTD0,	
PTD1,
PTD2,
PTD3,
PTD4,
PTD5,
PTD6,
PTD7
};

enum GPIOB {
PTE0,	
PTE1,
PTE2,
PTE3,
PTE4,
PTE5,
PTE6,
PTE7,
PTF0,	
PTF1,
PTF2,
PTF3,
PTF4,
PTF5,
PTF6,
PTF7,
PTG0,	
PTG1,
PTG2,
PTG3,
PTG4,
PTG5,
PTG6,
PTG7,
PTH0,	
PTH1,
PTH2,
PTH3,
PTH4,
PTH5,
PTH6,
PTH7
};

enum GPIOC {
PTI0,	
PTI1,
PTI2,
PTI3,
PTI4,
PTI5,
PTI6,
};
/*
#define A  A
#define B  A
#define C  A
#define D  A

#define E	B
#define F	B
#define G	B
#define H	B

#define I	C
*/
#define OUTPUT  1
#define INPUT	0

#define CONFIG_PIN_AS_GPIO(port,port_pin,mode)    XCONFIG_PIN_AS_GPIO(port,port_pin,mode)
#define XCONFIG_PIN_AS_GPIO(port,port_pin,mode)   (mode == 0) ? (GPIO##port##_PDDR |= 0 << port_pin) : (GPIO##port##_PDDR |= 1 << port_pin) 

#define ENABLE_INPUT(port,port_pin) 				XENABLE_INPUT(port,port_pin)
#define XENABLE_INPUT(port,port_pin)				GPIO##port##_PIDR ^= 1<<port_pin

#define ENABLE_PULLUP(port,port_pin) 				XENABLE_PULLUP(port,port_pin) 	
#define XENABLE_PULLUP(port,port_pin) 				PORT_PUE0 |= PORT_PUE0_PT##port##PE##port_pin##_MASK 

#define OUTPUT_SET(port,register_num)				XOUTPUT_SET(port,register_num)
#define XOUTPUT_SET(port,register_num)				GPIO##port##_PSOR |=1<<register_num

#define OUTPUT_CLEAR(port,register_num)				XOUTPUT_CLEAR(port,register_num)		
#define XOUTPUT_CLEAR(port,register_num)			GPIO##port##_PCOR |=1<<register_num

#define OUTPUT_TOGGLE(port,register_num)			XOUTPUT_TOGGLE(port,register_num)
#define XOUTPUT_TOGGLE(port,register_num)			GPIO##port##_PTOR |=1<<register_num

//
#define INPUT_GET(port,register_num)	 			(GPIO##port##_PDIR & (1<<register_num))
//
#define CONFIG_PIN_AS_FGPIO(port,port_pin,mode)    XCONFIG_PIN_AS_FGPIO(port,port_pin,mode)
#define XCONFIG_PIN_AS_FGPIO(port,port_pin,mode)   (mode == 0) ? (FGPIO##port##_PDDR |= 0 << port_pin) : (FGPIO##port##_PDDR |= 1 << port_pin) 

#endif /* GPIO_H_ */
