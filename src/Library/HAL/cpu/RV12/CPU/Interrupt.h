////////////////////////////////////////////////////////////////////
//   ,------.                    ,--.                ,--.         //
//   |  .--. ' ,---.  ,--,--.    |  |    ,---. ,---. `--' ,---.   //
//   |  '--'.'| .-. |' ,-.  |    |  |   | .-. | .-. |,--.| .--'   //
//   |  |\  \ ' '-' '\ '-'  |    |  '--.' '-' ' '-' ||  |\ `--.   //
//   `--' '--' `---'  `--`--'    `-----' `---' `-   /`--' `---'   //
//                                             `---'              //
//                                                                //
//      Interrupt header file                                     //
//		For the Roa Logic RV processor and plic                   //
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

#ifndef INTERRUPT_INTERRUPT_H_
#define INTERRUPT_INTERRUPT_H_

#include <rvl_SoC.h>

typedef void (*TimerInterruptEvent)(); //! Interface function for timer interrupt handlers
typedef void (*GPIOInterruptEvent)();  //! Interface function for GPIO interrupt handlers

extern TimerInterruptEvent _timerEvent[NUM_TIMERS]; //! Timer event array

/**
 * @brief Enable the global interrupt flag
 */
inline void enableGlobalInterrupt()
{
	set_csr(mstatus, MSTATUS_MIE);
}

/**
 * @brief Disable the global interrupt flag
 */
inline void disableGlobalInterrupt()
{
	clear_csr(mstatus, MSTATUS_MIE);
}

/**
 * @brief Enable the external interrupt flag
 */
inline void enableExternalInterrupt()
{
	set_csr(mie, 1 << IRQ_M_EXT);
}

/**
 * @brief Disable the external interrupt flag
 */
inline void disableExternalInterrupt()
{
	clear_csr(mie, 1 << IRQ_M_EXT);
}

/**
 * @brief Enable the global timer interrupt flag
 */
inline void enableTimerInterrupt()
{
	set_csr(mie, 1 << IRQ_M_TIMER);
}

/**
 * @brief Disable the global timer interrupt flag
 */
inline void disableTimerInterrupt()
{
	clear_csr(mie, 1 << IRQ_M_TIMER);
}



void trapHandler() __attribute__ ((aligned(4))) __attribute__ ((interrupt));

void interruptHandler(uint32_t cause);

void exceptionHandler(uint32_t cause);

//void handleExternalInterrupt();

#endif /* INTERRUPT_INTERRUPT_H_ */
