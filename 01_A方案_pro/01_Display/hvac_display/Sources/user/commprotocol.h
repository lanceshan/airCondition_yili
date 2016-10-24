/*
 * commprotocol.h
 *
 *  Created on: Sep 15, 2015
 *      Author: Hill
 */

#ifndef COMMPROTOCOL_H_
#define COMMPROTOCOL_H_

#include "can.h"
#include "derivative.h"


uint8 CAN_Rx(void);
void CAN_Tx(uint8 *CANTx_Buf);

#endif /* COMMPROTOCOL_H_ */
