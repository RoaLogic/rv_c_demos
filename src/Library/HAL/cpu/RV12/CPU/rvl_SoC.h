////////////////////////////////////////////////////////////////////
//   ,------.                    ,--.                ,--.         //
//   |  .--. ' ,---.  ,--,--.    |  |    ,---. ,---. `--' ,---.   //
//   |  '--'.'| .-. |' ,-.  |    |  |   | .-. | .-. |,--.| .--'   //
//   |  |\  \ ' '-' '\ '-'  |    |  '--.' '-' ' '-' ||  |\ `--.   //
//   `--' '--' `---'  `--`--'    `-----' `---' `-   /`--' `---'   //
//                                             `---'              //
//                                                                //
//      RVL SoC header file                                       //
//		For the Roa Logic RVL SoC	     		                  //
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

#ifndef CPU_RVL_CPU_H_
#define CPU_RVL_CPU_H_
#include <stdio.h>
#include "encoding.h"

#define AHB_CLOCK 100000000		//! AHB clock frequency
#define APB_CLOCK 25000000		//! APB clock frequency

#define NUM_GPIO_PORTS 2		//! Number of GPIO PORTS
#define PIN_NUM_MAX 8			//! number of pins per GPIO

#define NUM_TIMERS 3			//! number of timers

#if __riscv_xlen == 32
	#define MCAUSE_INT  	0x800000000UL
	#define MCAUSE_CAUSE	0x0000003FFUL
#else
	#define MCAUSE_INT  	0x80000000000000000UL
	#define MCAUSE_CAUSE	0x000000000000003FFUL
#endif

/**
 *	@brief Structure of GPIO registers
 */
typedef struct
{
	volatile uint8_t mode;				//! GPIO mode selection register
	volatile uint8_t direction;			//! GPIO direction register
	volatile uint8_t output;			//! GPIO output register
	volatile uint8_t input;				//! GPIO input register
	volatile uint8_t triggerType;		//! GPIO interrupt trigger register
	volatile uint8_t triggerLvl0;		//! GPIO interrupt trigger level 0
	volatile uint8_t triggerLvl1;		//! GPIO interrupt trigger level 1
	volatile uint8_t triggerStatus;		//! GPIO interrupt trigger status
	volatile uint8_t irqEnable;			//! GPIO interrupt enable register
}HAL_GPIO_Type;

#define GPIO0BASE ((HAL_GPIO_Type*)0x00400000)	//! GPIO 0 base
#define GPIO1BASE ((HAL_GPIO_Type*)0x00400010)	//! GPIO 1 base
#define GPIO2BASE ((HAL_GPIO_Type*)0x00400020)  //! GPIO 2 base
#define GPIO3BASE ((HAL_GPIO_Type*)0x00400030)  //! GPIO 3 base
#define GPIO4BASE ((HAL_GPIO_Type*)0x00400040)  //! GPIO 4 base
#define GPIO5BASE ((HAL_GPIO_Type*)0x00400050)  //! GPIO 5 base
#define GPIO6BASE ((HAL_GPIO_Type*)0x00400060)  //! GPIO 6 base
#define GPIO7BASE ((HAL_GPIO_Type*)0x00400070)  //! GPIO 7 base
#define GPIO8BASE ((HAL_GPIO_Type*)0x00400080)  //! GPIO 8 base
#define GPIO9BASE ((HAL_GPIO_Type*)0x00400090)  //! GPIO 9 base

/**
 * @brief Structure of timer registers
 */
typedef struct
{
	volatile uint32_t prescaler;		//! Timer prescaler register
	volatile uint32_t reserved;		//! Reserved
	volatile uint32_t Ipending;		//! Timer interrupt pending
	volatile uint32_t Ienable;		//! Timer enable
	volatile uint64_t Time;			//! Timer time register
}HAL_TIMER_Type;

#define TIMERBASE ((HAL_TIMER_Type*)0x00200000)	//! Global timer base
#define TIMER0BASE 0x00200018			//! Start address first timer



#endif /* CPU_RVL_CPU_H_ */
