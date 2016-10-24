#ifndef __CAN_Master_H
#define __CAN_Master_H

#define CAN1_RX_BUFFER_INDEX 0
#define CAN1_TX_BUFFER_INDEX 1

void CAN_Init(void);

void CAN_Send(unsigned char buf[], unsigned char len, uint32_t messageID);

unsigned char CAN_Read(unsigned char buf[], uint32_t *pMessageID);

void CAN_Rx_Callback(void);

#endif

