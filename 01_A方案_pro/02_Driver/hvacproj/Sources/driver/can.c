#include "CAN1.h"
#include "can.h"
#include "cstdlib"
static LDD_TDeviceData *pCANDevice;
static LDD_CAN_TFrame canRxFrame;

void CAN_Init(void)
{
	pCANDevice = CAN1_Init(NULL);
	
	MSCAN_CANBTR1 = (MSCAN_CANBTR1_TSEG2(0x01) | MSCAN_CANBTR1_TSEG1(0x04));

	canRxFrame.MessageID = 0x00;
	canRxFrame.FrameType = LDD_CAN_DATA_FRAME;
	canRxFrame.Data = (uint8_t *)malloc(sizeof(uint8_t)*8);
	canRxFrame.Length = 0;
	canRxFrame.TimeStamp = 0;
	canRxFrame.LocPriority = 0;	

}

void CAN_Send(unsigned char buf[], unsigned char len, uint32_t messageID)
{
	static uint16_t stamp = 0;
	LDD_TError err;

	LDD_CAN_TFrame Frame;
	
	Frame.MessageID = messageID;
	Frame.FrameType = LDD_CAN_DATA_FRAME;
	Frame.Data = &buf[0];
	Frame.Length = len;
	Frame.TimeStamp = stamp++;
	Frame.LocPriority = 0;
	
	if(pCANDevice != NULL)
	{
		err = CAN1_SendFrame(pCANDevice, CAN1_TX_BUFFER_INDEX, &Frame);
		if(err != ERR_OK)
		{
			//printf("can tx err = 0x%x\n", err);
		}
	}
}

unsigned char CAN_Read(unsigned char buf[], uint32_t *pMessageID)
{
	unsigned char i = 0;
	
	if(canRxFrame.Length == 0) return 0;
	if(canRxFrame.FrameType != LDD_CAN_DATA_FRAME) return 0;

	*pMessageID = canRxFrame.MessageID;
	for(i = 0;i < canRxFrame.Length;i++) buf[i] = canRxFrame.Data[i];

	canRxFrame.Length = 0;

	return i;
}

void CAN_Rx_Callback(void)
{
	unsigned char i = 0 ;
	LDD_TError err;
	uint8_t buf[8];
	LDD_CAN_TFrame Frame;
	Frame.Data=&buf[0];
	if(pCANDevice != NULL)
	{
		err = CAN1_ReadFrame(pCANDevice, CAN1_RX_BUFFER_INDEX, &Frame);
		if(err == ERR_OK)
		{
			canRxFrame.MessageID = Frame.MessageID;
			canRxFrame.FrameType = Frame.FrameType;
			canRxFrame.Length = Frame.Length;
			canRxFrame.TimeStamp = Frame.TimeStamp;
			canRxFrame.LocPriority = Frame.LocPriority;	
	
			for(i=0; i<Frame.Length; i++) 
			{
				canRxFrame.Data[i] = Frame.Data[i];
				//tt=Frame.Data[i];
			}
		}
		else
		{
			//printf("can rx err = 0x%x\n", err);
		}
	}
	return;	
}





