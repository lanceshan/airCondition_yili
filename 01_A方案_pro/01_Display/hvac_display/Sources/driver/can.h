/*
 * can.h
 *
 *  Created on: Sep 15, 2015
 *      Author: Hill
 */

#ifndef CAN_H_
#define CAN_H_

void CAN_Init(void);
void MSCAN_Rx_IRQHandler(void);
void MSCAN_Tx_IRQHandler(void);
void MSCAN_ModuleEn(void);
void Transmit_DataFrame(unsigned char len, unsigned char *Txbuf);
unsigned char Receive_DataFrame(unsigned char *data_rec);

#endif /* CAN_H_ */
