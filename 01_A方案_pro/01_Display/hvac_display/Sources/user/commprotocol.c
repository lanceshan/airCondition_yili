#include "commprotocol.h"
#include "globalvars.h"
#include "gpio.h"
uint8 CAN_Rx(void)//����IDΪ0x14A
{	
	uint8 CANRx_Buf[8];
	uint8 datalen = Receive_DataFrame(CANRx_Buf);	//�Զ��ж�CANID����0x14A������
	if(gCANRxFailTime >= MAX_CAN_FAIL_TIME)
		gSys_Info.Errorflags.bits.CANFault = 1;
	else
		gSys_Info.Errorflags.bits.CANFault = 0;
	if(datalen != 8)
		return datalen;	
	gCANRxFailTime = 0;
	switch(CANRx_Buf[0])		//�¶����ݷ����η��ͣ�����λ��С��λ�ֿ�
	{
		case 0x71:
			OUTPUT_TOGGLE(A,PTA1);
			gSys_Info.BusInTmp   = (float)((CANRx_Buf[1]&0x7f) + 0.1 * CANRx_Buf[2]);	
			gSys_Info.BusOutTmp  = (float)((CANRx_Buf[3]&0x7f) + 0.1 * CANRx_Buf[4]);
			gSys_Info.InPipeTmp  = (float)((CANRx_Buf[5]&0x7f) + 0.1 * CANRx_Buf[6]);
			gSys_Info.ErrPower.Byte = CANRx_Buf[7];
			if((CANRx_Buf[1] & 0x80) == 0x80)		//�ж����ݷ���λ
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
				gUser_Info->WindLv = CANRx_Buf[3];//�Զ�ģʽ�£�������ʾϵͳ��ǰ��������
			break;
		default:
			break;
	}
	return datalen;
}

void CAN_Tx(uint8 *CANTx_Buf)//����IDΪ0x14B
{	
	Transmit_DataFrame(8,CANTx_Buf);
}

/*
 	unsigned char cmd;
	unsigned char Temper;		//�û����õ��¶�
	DRV_MODE	  Mode;			//�û����õ�ģʽ
	unsigned char WindLv;		//�û����õķ����ȼ�
	unsigned char FreshAir;		//�û����õ��·�״̬
	unsigned char TestIndex;	//���ԣ�1��ѹ�������䣬2��ѹ�������ȣ�3��PTC����
	unsigned char Power; 
*/
