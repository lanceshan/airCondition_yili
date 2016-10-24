#include "commprotocol.h"
#include "globalvars.h"
#include "gpio.h"
uint8 CAN_Rx(void)//接收ID为0x14A
{	
	uint8 CANRx_Buf[8];
	uint8 datalen = Receive_DataFrame(CANRx_Buf);	//自动判断CANID，非0x14A不接收
	if(gCANRxFailTime >= MAX_CAN_FAIL_TIME)
		gSys_Info.Errorflags.bits.CANFault = 1;
	else
		gSys_Info.Errorflags.bits.CANFault = 0;
	if(datalen != 8)
		return datalen;	
	gCANRxFailTime = 0;
	switch(CANRx_Buf[0])		//温度数据分两次发送，整数位与小数位分开
	{
		case 0x71:
			OUTPUT_TOGGLE(A,PTA1);
			gSys_Info.BusInTmp   = (float)((CANRx_Buf[1]&0x7f) + 0.1 * CANRx_Buf[2]);	
			gSys_Info.BusOutTmp  = (float)((CANRx_Buf[3]&0x7f) + 0.1 * CANRx_Buf[4]);
			gSys_Info.InPipeTmp  = (float)((CANRx_Buf[5]&0x7f) + 0.1 * CANRx_Buf[6]);
			gSys_Info.ErrPower.Byte = CANRx_Buf[7];
			if((CANRx_Buf[1] & 0x80) == 0x80)		//判断数据符号位
				gSys_Info.BusInTmpSign = 1;
			else
				gSys_Info.BusInTmpSign = 0;
			if((CANRx_Buf[3] & 0x80) == 0x80)
				gSys_Info.BusOutTmpSign = 1;
			else
				gSys_Info.BusOutTmpSign = 0;
			if((CANRx_Buf[5] & 0x80) == 0x80)
				gSys_Info.InPipeTmpSign = 1;
			else
				gSys_Info.InPipeTmpSign = 0;
			break;
		case 0x72:
			gSys_Info.OutPipeTmp = (float)((CANRx_Buf[1]&0x7f) + 0.1 * CANRx_Buf[2]);
			gSys_Info.StoreTmp   = (float)((CANRx_Buf[3]&0x7f) + 0.1 * CANRx_Buf[4]);
			gSys_Info.CompTmp    = (float)((CANRx_Buf[5]&0x7f) + 0.1 * CANRx_Buf[6]);
			gSys_Info.Errorflags.Byte = CANRx_Buf[7];
			if((CANRx_Buf[1] & 0x80) == 0x80)
				gSys_Info.OutPipeTmpSign = 1;
			else
				gSys_Info.OutPipeTmpSign = 0;
			if((CANRx_Buf[3] & 0x80) == 0x80)
				gSys_Info.StoreTmpSign = 1;
			else
				gSys_Info.StoreTmpSign = 0;
			if((CANRx_Buf[5] & 0x80) == 0x80)
				gSys_Info.CompTmpSign = 1;
			else
				gSys_Info.CompTmpSign = 0;
			break;
		case 0x73:
			gSys_Info.ErrTempers.Byte = CANRx_Buf[7];
			if((CANRx_Buf[4] > 0) && (gUser_Info->Mode == MODE_AUTO))
				gUser_Info->WindLv = CANRx_Buf[3];//自动模式下，风量显示系统当前工作风量
			break;
		default:
			break;
	}
	return datalen;
}

void CAN_Tx(uint8 *CANTx_Buf)//发送ID为0x14B
{	
	Transmit_DataFrame(8,CANTx_Buf);
}

/*
 	unsigned char cmd;
	unsigned char Temper;		//用户设置的温度
	DRV_MODE	  Mode;			//用户设置的模式
	unsigned char WindLv;		//用户设置的风量等级
	unsigned char FreshAir;		//用户设置的新风状态
	unsigned char TestIndex;	//测试，1：压缩机制冷，2：压缩机制热，3：PTC制热
	unsigned char Power; 
*/
