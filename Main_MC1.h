/*
 * Main_MC1.h
 *
 *  Created on: Dec 14, 2020
 *      Author: MRM
 */

#ifndef MAIN_MC1_H_
#define MAIN_MC1_H_

uint8 ready = 0x02;
uint8 message[32];
uint8 message1[32];
uint8 Pass[6];
uint8 wrong_Pass[32]= "Wrong Pass";

uint8 menu_Option;
uint8 temp[6];
uint8 ch;
uint8 flag1 = 0;

uint8 yes = '1';
uint8 no = '0';

unsigned int count = 0;

#endif /* MAIN_MC1_H_ */
