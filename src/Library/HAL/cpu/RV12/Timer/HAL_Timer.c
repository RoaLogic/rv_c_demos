////////////////////////////////////////////////////////////////////
//   ,------.                    ,--.                ,--.         //
//   |  .--. ' ,---.  ,--,--.    |  |    ,---. ,---. `--' ,---.   //
//   |  '--'.'| .-. |' ,-.  |    |  |   | .-. | .-. |,--.| .--'   //
//   |  |\  \ ' '-' '\ '-'  |    |  '--.' '-' ' '-' ||  |\ `--.   //
//   `--' '--' `---'  `--`--'    `-----' `---' `-   /`--' `---'   //
//                                             `---'              //
//                                                                //
//      HAL_Timer source file                                     //
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
//     This source file is free software; you can redistribute    //
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

#include "HAL_Timer.h"

/**
 * @details This functions sets the base frequency on which the time count register is updated
 *
 * Tick frequency is calculated out of the AHB_CLOCK/(prescaler+1)
 * So if the main clock is 100Mhz, and the prescaler is 3, the tick frequency is 100Mhz / 3 + 1 = 25Mhz
 *
 * @note It can only be written once after a reset. It must be written, else the time counter doesn't start counting
 *
 * @param timerBase	The base address of the timer module
 * @param frequency	The tick rate frequency
 */
void HAL_TIMER_configureTickFrequency(HAL_TIMER_Type* timerBase, uint32_t frequency)
{
	timerBase->prescaler = ((AHB_CLOCK / frequency) - 1);
}

/**
 * @details This function sets the compare value for the time compare
 *
 * When the Time register equals the value in the TIMECMP[n], the interrupt is fired that the timer has expired.
 * This function sets the comparevalue according to the current value of the Time register. So the given comparevalue
 * is the time that is needed from this moment untill the interrupt is fired.
 *
 *
 * @note timerNum are counted from 0, so timer1, is timerNum 0
 *
 * @param timerBase
 * @param interruptEnable
 * @param timerNum
 */
void HAL_TIMER_setTimeCmpCount(HAL_TIMER_Type* timerBase, uint32_t compareValue, uint8_t timerNum)
{
	uint64_t* regToWrite;

	if(timerNum < NUM_TIMERS)
	{
		regToWrite = (uint64_t*)(TIMER0BASE + (8 * timerNum));

		*(regToWrite) = (timerBase->Time + compareValue);
	}
}

/**
 * @details This function sets the call back event function for the timer interrupt
 *
 * When a timer interrupt occurs, the registered event is called
 *
 * @note The timer interrupt event must make sure that the timer event flag is cleared
 *
 *
 * @param aEvent	The callback event for the timer
 * @param timerNum  The timer number for which the event is
 */
void HAL_TIMER_setCallbackFunction(TimerInterruptEvent aEvent, uint8_t timerNum)
{
	if(timerNum < NUM_TIMERS)
	{
		_timerEvent[timerNum] = aEvent;
	}
}

/**
 * @details This function clears the call back event for the timer interrupt
 *
 *
 * @param timerNum  The timer number for which the event must be removed
 */
void HAL_TIMER_removeCallbackFunction(uint8_t timerNum)
{
	if(timerNum < NUM_TIMERS)
	{
		_timerEvent[timerNum] = NULL;
	}
}

/**
 * @details This function sets the interrupt for timer[n]
 *
 * @note Interrupts are counted from 0, so timer1-interrupt, is 0
 *
 * @param timerBase
 * @param timerNum
 */
void HAL_TIMER_enableInterrupt(HAL_TIMER_Type* timerBase, uint8_t timerNum)
{
	if(timerNum < NUM_TIMERS)
	{
		timerBase->Ienable |= (1 << timerNum);
	}
}

/**
 * @details This function sets the interrupt for timer[n]
 *
 * @note Interrupts are counted from 0, so timer1-interrupt, is 0
 *
 * @param timerBase
 * @param timerNum
 */
void HAL_TIMER_disableInterrupt(HAL_TIMER_Type* timerBase, uint8_t timerNum)
{
	if(timerNum < NUM_TIMERS)
	{
		timerBase->Ienable &= ~(1 << timerNum);
	}
}
