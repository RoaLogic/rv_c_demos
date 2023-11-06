////////////////////////////////////////////////////////////////////
//   ,------.                    ,--.                ,--.         //
//   |  .--. ' ,---.  ,--,--.    |  |    ,---. ,---. `--' ,---.   //
//   |  '--'.'| .-. |' ,-.  |    |  |   | .-. | .-. |,--.| .--'   //
//   |  |\  \ ' '-' '\ '-'  |    |  '--.' '-' ' '-' ||  |\ `--.   //
//   `--' '--' `---'  `--`--'    `-----' `---' `-   /`--' `---'   //
//                                             `---'              //
//                                                                //
//      Interrupt source file                                     //
//	For the Roa Logic RV processor and plic                   //
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

#include "Interrupt.h"

TimerInterruptEvent _timerEvent[NUM_TIMERS] = {};

/** @brief Trap handler for the CPU
 * @details Function is called when an interrupt or an exception occurs.
 *
 * It is checked what the cause of the function is and the appropiate handler
 * is called, where the event is then handled.
 *
 */
void trapHandler()
{
	unsigned long cause = read_csr(mcause);

	if(cause > 32)
	{
		interruptHandler(cause);
	}
	else
	{
		exceptionHandler(cause);
	}
}

/** @brief Interrupt handler
 * @details Function to handle all different interrupts
 *
 * CPU can be interrupt by different causes. Not all causes are valid for the
 * current CPU and are therefore ignored. (Those will also not be triggered)
 *
 * Function then detects which module triggers the interrupt and calls the
 * appropiate handler.
 *
 *
 * @param cause The interrupt cause
 */
void interruptHandler(unsigned long cause)
{
	uint32_t intPending;

	switch(cause & 0x0F)
	{
		case 7 : 			//! Machine timer interrupt
			intPending = TIMERBASE->Ipending;

			if((intPending & 0x01) && _timerEvent[0])
			{
				_timerEvent[0]();
			}

			if((intPending & 0x02) && _timerEvent[1])
			{
				_timerEvent[1]();
			}

			if((intPending & 0x04)&& _timerEvent[2])
			{
				_timerEvent[2]();
			}
			break;
		case 11: 			//! Machine external interrupt

			break;
		case 0 :			//! User software interrupt
		case 1 :			//! Supervisor software interrupt
		case 3 : 			//! Machine software interrupt
		case 4 : 			//! User timer interrupt
		case 5 : 			//! Supervisor timer interrupt
		case 8 : 			//! User external interrupt
		case 9 :			//! supervisor external interrupt
		default:
			break;
	}
}

/** @brief Exception handler
 *
 * @todo Add exception handling
 *
 * @param cause The exception cause
 */
void exceptionHandler(unsigned long cause)
{

}
