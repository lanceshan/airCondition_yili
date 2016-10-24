/* ###################################################################
**     Filename    : Events.c
**     Project     : hvacproj
**     Processor   : SKEAZ128MLK4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-07-20, 19:34, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Cpu_OnNMI - void Cpu_OnNMI(void);
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "Init_Config.h"
#include "PDD_Includes.h"
#include "user\globalvars.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Event       :  Timer_10ms_OnInterrupt (module Events)
**
**     Component   :  Timer_10ms [TimerInt_LDD]
*/
/*!
**     @brief
**         Called if periodic event occur. Component and OnInterrupt
**         event must be enabled. See [SetEventMask] and [GetEventMask]
**         methods. This event is available only if a [Interrupt
**         service/event] is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer passed as
**                           the parameter of Init method.
*/
/* ===================================================================*/
void Timer_10ms_OnInterrupt(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
	if(gSysStartTcnt > 0)gSysStartTcnt--;
	if(gSysRunTcnt > 0)gSysRunTcnt--;
	if(gMsgTxTcnt > 0)gMsgTxTcnt--;
	if(gHotToColdTcnt > 0)gHotToColdTcnt--;
	if(gColdToHotTcnt > 0)gColdToHotTcnt--;
	if(gGetSysInfoTcnt > 0)gGetSysInfoTcnt--;
	if(gTrendChgTcnt > 0)gTrendChgTcnt--;
	if(gOFFModeTcnt > 0)gOFFModeTcnt--;
	if(gDCErrCheckTcnt > 0)gDCErrCheckTcnt--;
	if(gPowerErrCheckTcnt > 0)gPowerErrCheckTcnt--;
	if(sys_Ctrl.CompOffStepTime > 0)sys_Ctrl.CompOffStepTime--;
	if(sys_Ctrl.InnerFanOnTime > 0)sys_Ctrl.InnerFanOnTime--;

	
	if((gcur_state == state_comphot) ||	(gcur_state == state_bothhot))
	{
		sys_Ctrl.CompHotTime++;
	}
	else
	{
		sys_Ctrl.CompHotTime = 0;
	}
	if(gcur_state == state_frost)	
		gDefrostTcnt++;
	else 						   	
		gDefrostTcnt = 0;
	if(sys_Ctrl.CompLv == 0)
	{
		sys_Ctrl.CompOnEn = 0;
		if(sys_Ctrl.CompOffTime < 10*TIME_MIN)sys_Ctrl.CompOffTime++;
		if(sys_Ctrl.CompOffTime > TIME_MIN)sys_Ctrl.CompOnEn = 1;
		sys_Ctrl.CompOnTime = 0;
	}
	else  	
	{
		sys_Ctrl.CompOffTime = 0;
		if(sys_Ctrl.CompOnTime < 10*TIME_MIN)sys_Ctrl.CompOnTime++;
	}
	
	if(sys_Ctrl.CompLv >= 5)
	{
		if(sys_Ctrl.CompHiLvTime < (3*TIME_MIN))
			sys_Ctrl.CompHiLvTime++;
	}
	else
	{
		sys_Ctrl.CompHiLvTime = 0;
	}
	
	if(sys_Ctrl.CompHiLvTime >= (3*TIME_MIN))
		sys_Ctrl.CompHiLvEn = 1;
	else
		sys_Ctrl.CompHiLvEn = 0;
	
	
	if(sys_Info.OutPipeTmp <= -5)
	{
		if(gFrostLowTempTcnt < (35*TIME_SEC))
			gFrostLowTempTcnt++;
	}
	else
	{
		gFrostLowTempTcnt = 0;	
	}
	
	if(gSysRunTcnt == 0)
	{
		LED_NegVal(NULL);
		gSysRunTcnt = TIME_SEC;
	}
}

/*
** ===================================================================
**     Event       :  Timer_1ms_OnInterrupt (module Events)
**
**     Component   :  Timer_1ms [TimerInt_LDD]
*/
/*!
**     @brief
**         Called if periodic event occur. Component and OnInterrupt
**         event must be enabled. See [SetEventMask] and [GetEventMask]
**         methods. This event is available only if a [Interrupt
**         service/event] is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer passed as
**                           the parameter of Init method.
*/
/* ===================================================================*/
void Timer_1ms_OnInterrupt(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */	
	if(gRightFreshWks_Tcnt > 0)gRightFreshWks_Tcnt--;
	if(gLeftFreshWks_Tcnt > 0)gLeftFreshWks_Tcnt--;
	
	if(gRightFreshWks_Tcnt != 0)
		RightFresh_CLK_NegVal();		//生成新风门控制信号PWM
	if(gLeftFreshWks_Tcnt != 0) 
		LeftFresh_CLK_NegVal();
}

/*
** ===================================================================
**     Event       :  CAN1_OnFreeTxBuffer (module Events)
**
**     Component   :  CAN1 [CAN_LDD]
*/
/*!
**     @brief
**         This event is called when the buffer is empty after a
**         successful transmit of a message. This event is available
**         only if method SendFrame is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
**     @param
**         BufferIdx       - Receive message buffer index.
*/
/* ===================================================================*/
void CAN1_OnFreeTxBuffer(LDD_TUserData *UserDataPtr, LDD_CAN_TMBIndex BufferIdx)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  CAN1_OnFullRxBuffer (module Events)
**
**     Component   :  CAN1 [CAN_LDD]
*/
/*!
**     @brief
**         This event is called when the buffer is full after a
**         successful receive a message. This event is available only
**         if method ReadFrame or SetRxBufferState is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
**     @param
**         BufferIdx       - Transmit buffer index.
*/
/* ===================================================================*/
void CAN1_OnFullRxBuffer(LDD_TUserData *UserDataPtr, LDD_CAN_TMBIndex BufferIdx)
{
  /* Write your code here ... */
	if(MSCAN_CANRFLG & 0x01)
	{
	  	CAN_Rx_Callback();
		MSCAN_CANRFLG = 0;
	}
}

/*
** ===================================================================
**     Event       :  IIC_OnMasterBlockSent (module Events)
**
**     Component   :  IIC [I2C_LDD]
*/
/*!
**     @brief
**         This event is called when I2C in master mode finishes the
**         transmission of the data successfully. This event is not
**         available for the SLAVE mode and if MasterSendBlock is
**         disabled. 
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void IIC_OnMasterBlockSent(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  IIC_OnMasterBlockReceived (module Events)
**
**     Component   :  IIC [I2C_LDD]
*/
/*!
**     @brief
**         This event is called when I2C is in master mode and finishes
**         the reception of the data successfully. This event is not
**         available for the SLAVE mode and if MasterReceiveBlock is
**         disabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void IIC_OnMasterBlockReceived(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
}

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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
