/* ###################################################################
**     Filename    : Events.h
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
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
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
#include "driver\can.h"
#ifdef __cplusplus
extern "C" {
#endif 

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
void CAN1_OnFreeTxBuffer(LDD_TUserData *UserDataPtr, LDD_CAN_TMBIndex BufferIdx);

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
void CAN1_OnFullRxBuffer(LDD_TUserData *UserDataPtr, LDD_CAN_TMBIndex BufferIdx);

/*
** ===================================================================
**     Event       :  Timer_10ms_OnInterrupt (module Events)
**
**     Component   :  Timer_10ms [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void Timer_10ms_OnInterrupt(LDD_TUserData *UserDataPtr);

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
void IIC_OnMasterBlockSent(LDD_TUserData *UserDataPtr);

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
void IIC_OnMasterBlockReceived(LDD_TUserData *UserDataPtr);

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
void Timer_1ms_OnInterrupt(LDD_TUserData *UserDataPtr);

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
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
