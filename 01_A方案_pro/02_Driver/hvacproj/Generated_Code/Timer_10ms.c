/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : Timer_10ms.c
**     Project     : hvacproj
**     Processor   : SKEAZ128MLK4
**     Component   : TimerInt_LDD
**     Version     : Component 01.018, Driver 01.02, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-07-19, 10:16, # CodeGen: 258
**     Abstract    :
**          This TimerInt component implements a periodic interrupt.
**          When the component and its events are enabled, the "OnInterrupt"
**          event is called periodically with the period that you specify.
**          TimerInt supports also changing the period in runtime.
**          This TimerInt component provides a high level API for unified
**          hardware access to various timer devices using the TimerUnit
**          component.
**     Settings    :
**          Component name                                 : Timer_10ms
**          Periodic interrupt source                      : FTM0_C1V
**          Counter                                        : FTM0_CNT
**          Interrupt service/event                        : Enabled
**            Interrupt                                    : INT_FTM0
**            Interrupt priority                           : medium priority
**          Interrupt period                               : 20 ms
**          Initialization                                 : 
**            Enabled in init. code                        : yes
**            Auto initialization                          : yes
**            Event mask                                   : 
**              OnInterrupt                                : Enabled
**          CPU clock/configuration selection              : 
**            Clock configuration 0                        : This component enabled
**            Clock configuration 1                        : This component disabled
**            Clock configuration 2                        : This component disabled
**            Clock configuration 3                        : This component disabled
**            Clock configuration 4                        : This component disabled
**            Clock configuration 5                        : This component disabled
**            Clock configuration 6                        : This component disabled
**            Clock configuration 7                        : This component disabled
**          Referenced components                          : 
**            Linked TimerUnit                             : TU2
**     Contents    :
**         Init - LDD_TDeviceData* Timer_10ms_Init(LDD_TUserData *UserDataPtr);
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file Timer_10ms.c
** @version 01.02
** @brief
**          This TimerInt component implements a periodic interrupt.
**          When the component and its events are enabled, the "OnInterrupt"
**          event is called periodically with the period that you specify.
**          TimerInt supports also changing the period in runtime.
**          This TimerInt component provides a high level API for unified
**          hardware access to various timer devices using the TimerUnit
**          component.
*/         
/*!
**  @addtogroup Timer_10ms_module Timer_10ms module documentation
**  @{
*/         

/* MODULE Timer_10ms. */

#include "Events.h"
#include "Timer_10ms.h"
/* {Default RTOS Adapter} No RTOS includes */

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct {
  LDD_TDeviceData *LinkedDeviceDataPtr;
  bool EnUser;                         /* Enable/Disable device */
  LDD_TEventMask EnEvents;             /* Enable/Disable events mask */
  uint16_t CmpVal;                     /* Value periodically addded to compare register */
  LDD_TUserData *UserDataPtr;          /* RTOS device data structure */
} Timer_10ms_TDeviceData;

typedef Timer_10ms_TDeviceData *Timer_10ms_TDeviceDataPtr; /* Pointer to the device data structure. */

/* {Default RTOS Adapter} Static object used for simulation of dynamic driver memory allocation */
static Timer_10ms_TDeviceData DeviceDataPrv__DEFAULT_RTOS_ALLOC;

#define CHANNEL 0x00U
#define AVAILABLE_EVENTS_MASK (LDD_TEventMask)(LDD_TIMERINT_ON_INTERRUPT)
/* Internal method prototypes */
/*
** ===================================================================
**     Method      :  Timer_10ms_Init (component TimerInt_LDD)
*/
/*!
**     @brief
**         Initializes the device. Allocates memory for the device data
**         structure, allocates interrupt vectors and sets interrupt
**         priority, sets pin routing, sets timing, etc. If the
**         property ["Enable in init. code"] is set to "yes" value then
**         the device is also enabled (see the description of the
**         [Enable] method). In this case the [Enable] method is not
**         necessary and needn't to be generated. This method can be
**         called only once. Before the second call of Init the [Deinit]
**         must be called first.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an event or callback parameter.
**     @return
**                         - Pointer to the dynamically allocated private
**                           structure or NULL if there was an error.
*/
/* ===================================================================*/
LDD_TDeviceData* Timer_10ms_Init(LDD_TUserData *UserDataPtr)
{
  /* Allocate device structure */
  Timer_10ms_TDeviceData *DeviceDataPrv;
  /* {Default RTOS Adapter} Driver memory allocation: Dynamic allocation is simulated by a pointer to the static object */
  DeviceDataPrv = &DeviceDataPrv__DEFAULT_RTOS_ALLOC;
  DeviceDataPrv->UserDataPtr = UserDataPtr; /* Store the RTOS device structure */
  DeviceDataPrv->EnEvents = 0x01u;     /* Initial event mask */
  DeviceDataPrv->EnUser = TRUE;        /* Set the flag "device enabled" */
  DeviceDataPrv->CmpVal = 0xEA60U;     /* Initial value periodically addded to compare register */
  /* Registration of the device structure */
  PE_LDD_RegisterDeviceStructure(PE_LDD_COMPONENT_Timer_10ms_ID,DeviceDataPrv);
  DeviceDataPrv->LinkedDeviceDataPtr = TU2_Init((LDD_TUserData *)NULL);
  if (DeviceDataPrv->LinkedDeviceDataPtr == NULL) { /* Is initialization of TimerUnit unsuccessful? */
    /* Unregistration of the device structure */
    PE_LDD_UnregisterDeviceStructure(PE_LDD_COMPONENT_Timer_10ms_ID);
    /* Deallocation of the device structure */
    /* {Default RTOS Adapter} Driver memory deallocation: Dynamic allocation is simulated, no deallocation code is generated */
    return NULL;                       /* If so, then the TimerInt initialization is also unsuccessful */
  }
  (void)TU2_SetEventMask(DeviceDataPrv->LinkedDeviceDataPtr, TU2_GetEventMask(DeviceDataPrv->LinkedDeviceDataPtr) | LDD_TIMERUNIT_ON_CHANNEL_0); /* Enable TimerUnit event */
  return ((LDD_TDeviceData *)DeviceDataPrv); /* Return pointer to the device data structure */
}

/*
** ===================================================================
**     Method      :  TU2_OnChannel0 (component TimerInt_LDD)
**
**     Description :
**         The method services the event of the linked component TU2 and 
**         eventually invokes event Timer_10ms_OnInterrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void TU2_OnChannel0(LDD_TUserData *UserDataPtr)
{
  Timer_10ms_TDeviceData *DeviceDataPrv = PE_LDD_DeviceDataList[PE_LDD_COMPONENT_Timer_10ms_ID];
  uint16_t Ticks;

  (void)UserDataPtr;                   /* Parameter is not used, suppress unused argument warning */
  (void)TU2_GetOffsetTicks(DeviceDataPrv->LinkedDeviceDataPtr, CHANNEL, &Ticks);
  Ticks += DeviceDataPrv->CmpVal;
  (void)TU2_SetOffsetTicks(DeviceDataPrv->LinkedDeviceDataPtr, CHANNEL, Ticks);
  if (DeviceDataPrv->EnEvents & LDD_TIMERINT_ON_INTERRUPT) { /* Is the event enabled? */
    Timer_10ms_OnInterrupt(DeviceDataPrv->UserDataPtr); /* Invoke OnInterrupt event */
  }
}

/* END Timer_10ms. */

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
