/* ###################################################################
**     Filename    : main.c
**     Project     : hvacproj
**     Processor   : SKEAZ128MLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-07-20, 19:34, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */

/* Including needed modules to compile this module/procedure */

#include "Cpu.h"
#include "Events.h"
#include "CAN1.h"
#include "IIC.h"
#include "LED.h"
#include "OutsideFan_PWM.h"
#include "PwmLdd1.h"
#include "TU1.h"
#include "InnerFan_PWM.h"
#include "PwmLdd2.h"
#include "Timer_10ms.h"
#include "TU2.h"
#include "VFDRun_dFault.h"
#include "VFD_ON_OFF.h"
#include "BitIoLdd1.h"
#include "VFD_FM1.h"
#include "BitIoLdd2.h"
#include "VFD_FM2.h"
#include "BitIoLdd3.h"
#include "VFD_FM3.h"
#include "BitIoLdd4.h"
#include "InnerFan_dFault.h"
#include "BitsIoLdd1.h"
#include "OutsideFan_dFault.h"
#include "BitsIoLdd2.h"
#include "Timer_1ms.h"
#include "TU3.h"
#include "VFDRun_dFlag.h"
#include "BitIoLdd5.h"
#include "BitIoLdd6.h"
#include "PTC_Ctrl.h"
#include "BitIoLdd7.h"
#include "Valve_Ctrl.h"
#include "BitIoLdd9.h"
#include "StoreFan_Ctrl.h"
#include "BitIoLdd21.h"
#include "Master_Ctrl.h"
#include "BitIoLdd11.h"
#include "BitIoLdd12.h"
#include "LeftFresh_Photo.h"
#include "BitIoLdd23.h"
#include "LeftFresh_Enable.h"
#include "BitIoLdd8.h"
#include "RightFresh_Photo.h"
#include "BitIoLdd22.h"
#include "CompOverPress_dFault.h"
#include "BitIoLdd13.h"
#include "CompLowPress_dFault.h"
#include "BitIoLdd14.h"
#include "TravelSwitch_dFault.h"
#include "PowerOver_dFault.h"
#include "BitIoLdd17.h"
#include "PowerLow_dFault.h"
#include "BitIoLdd18.h"
#include "LeftFresh_CLK.h"
#include "BitIoLdd15.h"
#include "LeftFresh_Dir.h"
#include "BitIoLdd16.h"
#include "RightFresh_CLK.h"
#include "BitIoLdd19.h"
#include "RightFresh_Dir.h"
#include "BitIoLdd20.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"

#include "driver\can.h"
#include "driver\adc.h"
#include "function\compressor.h"
#include "function\fan.h"
#include "function\ptc.h"
#include "function\valve.h"
#include "function\power.h"
#include "user\globalvars.h"
#include "user\commprotocol.h"
#include "user\getSysInfo.h"
#include "user\mode.h"
#include "user\error.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
//LDD_TDeviceData *DeviceDataPtr;
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */

int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */
	DRV_MODE  cur_Mode = MODE_HALT;
	uint8 err_Mode = ERR_NULL;
	uint8 power_state = 0;
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  /*** End of Processor Expert internal initialization.                    ***/
	PE_low_level_init();
	CAN_Init();
	adc_init();
	
	while(gSysRunTcnt != 1);	//上电延时，确保系统稳定后读取传感器值
	SysInfo_Init();	
	Set_InnerFan_Speed(0);
	Set_OutsideFan_Speed(0);
	for(;;)
	{		
		CAN_Tx();								//每500ms发送一次数据
		CAN_Rx();								//数据接收处理
		Get_SysInfo();
		
		if(user_Info.Power == POWER_ON) 		//开机运行状态下动作
		{
			err_Mode = ErrorCheck();
			if(err_Mode == ERR_NULL)
			{
				power_state = 3;
				Power_ON();					
				cur_Mode = Mode_Switch(cur_Mode);
				ModeProcess(cur_Mode);
			}
			else
			{
				ErrorHandle(err_Mode);
			}
		}
		else									//关机处理
		{
			power_state = Power_OFFProcess(power_state);
		}
	}

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
