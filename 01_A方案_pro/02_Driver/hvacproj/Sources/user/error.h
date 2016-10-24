/*
 * error.h
 *
 *  Created on: Jul 30, 2015
 *      Author: Hill
 */

#ifndef ERROR_H_
#define ERROR_H_

#define ERR_NULL 			0x00 	 		//Œﬁπ ’œ
#define ERR_Lv1 			0x01 			
#define ERR_Lv2 			0x02 			
#define ERR_Lv3				0x04 			

unsigned char ErrorCheck(void);
void ErrorHandle(unsigned char err);
#endif /* ERROR_H_ */
