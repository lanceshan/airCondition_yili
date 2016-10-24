/*
 * display.h
 *
 *  Created on: Apr 20, 2016
 *      Author: Hill
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "ht1621.h"

void Display(void);
void Display_Test(void);
void Handle_DisBlinkBmp(unsigned char buf[][13]);
#endif /* DISPLAY_H_ */
