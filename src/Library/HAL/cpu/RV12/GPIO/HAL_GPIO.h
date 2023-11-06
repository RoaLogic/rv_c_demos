////////////////////////////////////////////////////////////////////
//   ,------.                    ,--.                ,--.         //
//   |  .--. ' ,---.  ,--,--.    |  |    ,---. ,---. `--' ,---.   //
//   |  '--'.'| .-. |' ,-.  |    |  |   | .-. | .-. |,--.| .--'   //
//   |  |\  \ ' '-' '\ '-'  |    |  '--.' '-' ' '-' ||  |\ `--.   //
//   `--' '--' `---'  `--`--'    `-----' `---' `-   /`--' `---'   //
//                                             `---'              //
//                                                                //
//      HAL_GPIO header file                                      //
//	For the Roa Logic apb4 gpio module                        //
//                                                                //
////////////////////////////////////////////////////////////////////
//                                                                //
//     Copyright (C) 2016-2021 ROA Logic BV                       //
//     www.roalogic.com                                           //
//                                                                //
//     This source file may be used and distributed without       //
//   restrictions, provided that this copyright statement is      //
//   not removed from the file and that any derivative work       //
//   contains the original copyright notice and the associated    //
//   disclaimer.                                                  //
//                                                                //
//     This soure file is free software; you can redistribute     //
//   it and/or modify it under the terms of the GNU General       //
//   Public License as published by the Free Software             //
//   Foundation, either version 3 of the License, or (at your     //
//   option) any later versions.                                  //
//   The current text of the License can be found at:             //
//   http://www.gnu.org/licenses/gpl.html                         //
//                                                                //
//     This source file is distributed in the hope that it will   //
//   be useful, but WITHOUT ANY WARRANTY; without even the        //
//   implied warranty of MERCHANTABILITY or FITTNESS FOR A        //
//   PARTICULAR PURPOSE. See the GNU General Public License for   //
//   more details.                                                //
//                                                                //
////////////////////////////////////////////////////////////////////

#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <rvl_SoC.h>

#define INPUT 0
#define OUTPUT 1

#define PUSH_PULL 0
#define OPEN_DRAIN 1

#define LEVEL_TRIGGERED 0
#define EDGE_TRIGGERED  1

#define NOT_TRIGGERED 			0
#define FALLING_EDGE_TRIGGERED  1
#define RISING_EDGE_TRIGGERED   2
#define BOTH_EDGE_TRIGGERED 	3

#define NO_LEVEL_TRIGGERED		0
#define HIGH_LEVEL_TRIGGERED 	1
#define LOW_LEVEL_TRIGGERED		2


void HAL_GPIO_setRegPinMode(HAL_GPIO_Type* baseAddress, uint8_t pinSetting);
void HAL_GPIO_setRegPinDirection(HAL_GPIO_Type* baseAddress, uint8_t pinSetting);

void HAL_GPIO_setPinMode(HAL_GPIO_Type* baseAddress, uint8_t pinNum, uint8_t pinSetting);
void HAL_GPIO_setPinDirection(HAL_GPIO_Type* baseAddress, uint8_t pinNum, uint8_t pinSetting);

void HAL_GPIO_enablePinInterrupt(HAL_GPIO_Type* baseAddress, uint8_t pinNum);
void HAL_GPIO_disablePinInterrupt(HAL_GPIO_Type* baseAddress, uint8_t pinNum);

void HAL_GPIO_setPinInterrupt(HAL_GPIO_Type* baseAddress, uint8_t pinNum, uint8_t triggerType, uint8_t triggerSetting);

uint8_t HAL_GPIO_getTriggerStatus(HAL_GPIO_Type* basePin);

void HAL_GPIO_setRegPinOutput(HAL_GPIO_Type* baseAddress, uint8_t pinSetting);
void HAL_GPIO_setPinOutput(HAL_GPIO_Type* baseAddress, uint8_t pinNum, uint8_t pinSetting);

void HAL_GPIO_togglePinOutput(HAL_GPIO_Type* baseAddress, uint8_t pinNum);

uint8_t HAL_GPIO_getRegPinInput(HAL_GPIO_Type* baseAddress);
uint8_t HAL_GPIO_getPinInput(HAL_GPIO_Type* baseAddress, uint8_t pinNum);

#endif
