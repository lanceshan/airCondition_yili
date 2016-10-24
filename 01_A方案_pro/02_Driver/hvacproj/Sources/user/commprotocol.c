/*
 * commprotocol.c
 *
 *  Created on: Jul 30, 2015
 *      Author: Hill
 */

#include "commprotocol.h"
#include "CAN1.h"
#include "driver\can.h"
#include "user\globalvars.h"
#include <cstdlib>

#include "function\fan.h"
#include "function\compressor.h"
#include "function\ptc.h"
#include "function\valve.h"
#include "function\power.h"

void CAN_Rx(void)
{
	if(CAN_Read(&CANRx_Buf[0], &CAN_ID) != 8)return;	//接收数据
	if(CAN_ID != 0x14b)return;
	sys_Info.CANRecvErrTimes = 0;
	switch(CANRx_Buf[0])
	{
		case 0x33:			//接收用户设置信息
			user_Info.cmd	   = CANRx_Buf[0];
			user_Info.Temper   = CANRx_Buf[1];
			user_Info.Mode     = CANRx_Buf[2];
			user_Info.WindLv   = CANRx_Buf[3];
			user_Info.FreshAir = CANRx_Buf[4];
			user_Info.PTC      = CANRx_Buf[5];
			user_Info.TestIndex= CANRx_Buf[6];
			user_Info.Power    = CANRx_Buf[7];
			break;
#if 0
		case 0x44:		//调试命令
			if((CANRx_Buf[1] >= 0) && (CANRx_Buf[1] <= 6))
				Set_InnerFan_Speed(CANRx_Buf[1]);
			if((CANRx_Buf[2] >= 0) && (CANRx_Buf[2] <= 2))
				Set_OutsideFan_Speed(CANRx_Buf[2]);
			
			if((CANRx_Buf[3] >= 0) && (CANRx_Buf[3] <= 8))
				Set_Cmp(CANRx_Buf[3]);
			if(CANRx_Buf[4] == 0)Set_PTC(0);	
			if(CANRx_Buf[4] == 1)Set_PTC(1);
			
			if(CANRx_Buf[5] == 0)Close_Valve();	
			if(CANRx_Buf[5] == 1)Open_Valve();	
			
			if(CANRx_Buf[6] == 0)Close_StoreFan();			//新风关	
			if(CANRx_Buf[6] == 1)Open_StoreFan();			//新风开		
			if(CANRx_Buf[7] == 0)Power_OFF();
			if(CANRx_Buf[7] == 1)Power_ON();	
			break;
#endif
		default:
			break;
	}
}

void CAN_Tx(void)
{
	static unsigned char sTx_Index = 0;
	if(gMsgTxTcnt != 0)return;
	gMsgTxTcnt = TIME_MSG_SEND;			//每隔500ms发送一次数据
	if(sys_Info.CANRecvErrTimes < 100)sys_Info.CANRecvErrTimes++;	
	switch (sTx_Index)
	{
		case 0:
			CANTx_Buf[0] = 0x71;		//发送车内回风、车外、左吹风温度，精度小数点后1位
			CANTx_Buf[1] = (uint8)((uint16)(abs(sys_Info.BusInTmp   *10))/10);
			CANTx_Buf[2] = (uint8)((uint16)(abs(sys_Info.BusInTmp   *10))%10);
			CANTx_Buf[3] = (uint8)((uint16)(abs(sys_Info.BusOutTmp  *10))/10);
			CANTx_Buf[4] = (uint8)((uint16)(abs(sys_Info.BusOutTmp  *10))%10);
			CANTx_Buf[5] = (uint8)((uint16)(abs(sys_Info.InPipeTmp *10))/10);
			CANTx_Buf[6] = (uint8)((uint16)(abs(sys_Info.InPipeTmp *10))%10);
			CANTx_Buf[7] = sys_Info.ErrPower.Byte;
			if(sys_Info.BusInTmp  < 0)CANTx_Buf[1] |= 0x80;
			if(sys_Info.BusOutTmp < 0)CANTx_Buf[3] |= 0x80;
			if(sys_Info.InPipeTmp< 0) CANTx_Buf[5] |= 0x80;
			CAN_ID = 0x14a;
			CAN_Send(CANTx_Buf,8,CAN_ID);
			sTx_Index++;
			break;
		case 1:
			CANTx_Buf[0] = 0x72;		//发送右吹风、除霜温度、MCU内部温度，精度小数点后1位
			CANTx_Buf[1] = (uint8)((uint16)(abs(sys_Info.OutPipeTmp *10))/10);
			CANTx_Buf[2] = (uint8)((uint16)(abs(sys_Info.OutPipeTmp *10))%10);
			CANTx_Buf[3] = (uint8)((uint16)(abs(sys_Info.StoreTmp   *10))/10);
			CANTx_Buf[4] = (uint8)((uint16)(abs(sys_Info.StoreTmp   *10))%10);
			CANTx_Buf[5] = (uint8)((uint16)(abs(sys_Info.CompTmp  *10))/10);
			CANTx_Buf[6] = (uint8)((uint16)(abs(sys_Info.CompTmp  *10))%10);
			CANTx_Buf[7] = sys_Info.ErrDevices.Byte;
			if(sys_Info.OutPipeTmp < 0)CANTx_Buf[1] |= 0x80;
			if(sys_Info.StoreTmp   < 0)CANTx_Buf[3] |= 0x80;
			if(sys_Info.CompTmp  < 0)  CANTx_Buf[5] |= 0x80;
			CAN_ID = 0x14a;						
			CAN_Send(CANTx_Buf,8,CAN_ID);		
			sTx_Index++;
			break;
		case 2:
			CANTx_Buf[0] = 0x73;		
			CANTx_Buf[1] = gcur_state;				//监测当前的工作状态
			CANTx_Buf[3] = sys_Ctrl.InnerFanLv;
			CANTx_Buf[4] = sys_Ctrl.OutsideFanLv;
			CANTx_Buf[2] = sys_Ctrl.CompLv;			//监测压缩机的工作频率		
			CANTx_Buf[5] = sys_Ctrl.PTCLv;
			CANTx_Buf[6] = 0;
			CANTx_Buf[7] = sys_Info.ErrTempers.Byte;
			CAN_ID = 0x14a;						
			CAN_Send(CANTx_Buf,8,CAN_ID);		
			sTx_Index = 0;
			break;
		default:
			break;
	}
}
