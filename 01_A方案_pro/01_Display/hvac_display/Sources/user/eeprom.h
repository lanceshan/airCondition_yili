/*
 * eeprom.h
 *
 *  Created on: Sep 15, 2015
 *      Author: Hill
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "iic.h"

uint8 Read_Prv_Info(uint8 *Buf, uint8 len);
void Save_Cur_Info(uint8 *Buf, uint8 len);

#endif /* EEPROM_H_ */
