////////////////////////////////////////////////////////////////////
//   ,------.                    ,--.                ,--.         //
//   |  .--. ' ,---.  ,--,--.    |  |    ,---. ,---. `--' ,---.   //
//   |  '--'.'| .-. |' ,-.  |    |  |   | .-. | .-. |,--.| .--'   //
//   |  |\  \ ' '-' '\ '-'  |    |  '--.' '-' ' '-' ||  |\ `--.   //
//   `--' '--' `---'  `--`--'    `-----' `---' `-   /`--' `---'   //
//                                             `---'              //
//                                                                //
//      HAL_Timer header file                                     //
//		For the Roa Logic ahb-Lite Timer module                   //
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
#ifndef TIMER_HAL_TIMER_H_
#define TIMER_HAL_TIMER_H_

#include <Interrupt.h>
#include <rvl_SoC.h>

void HAL_TIMER_configureTickFrequency(HAL_TIMER_Type* timerBase, uint32_t frequency);
void HAL_TIMER_setTimeCmpCount(HAL_TIMER_Type* timerBase, uint32_t compareValue, uint8_t timerNum);
void HAL_TIMER_setCallbackFunction(TimerInterruptEvent aEvent, uint8_t timerNum);
void HAL_TIMER_removeCallbackFunction(uint8_t timerNum);
void HAL_TIMER_enableInterrupt(HAL_TIMER_Type* timerBase, uint8_t timerNum);
void HAL_TIMER_disableInterrupt(HAL_TIMER_Type* timerBase, uint8_t timerNum);

inline uint64_t HAL_TIMER_getTimeRegister(HAL_TIMER_Type* timerBase)
{
	return timerBase->Time;
}
inline uint32_t HAL_TIMER_getInterruptState(HAL_TIMER_Type* timerBase)
{
	return timerBase->Ipending;
}

#endif /* TIMER_HAL_TIMER_H_ */
