/**
 * @file main.cpp
 * @author Bjorn Schouteten
 * @brief 
 * @version 0.1
 * @date 24-04-2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "Interrupt.h"
#include "HAL_GPIO.h"
#include "HAL_Timer.h"

#include <FreeRTOS.h>
#include <task.h>

#define LEDGPIO GPIO0BASE
#define LED0 0
#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4
#define LED5 5
#define LED6 6
#define LED7 7

extern void main_blinky( void );

/**
 * @brief Timer 2 handler function
 */
void handle_timer2()
{
	HAL_GPIO_togglePinOutput(LEDGPIO, LED3);
	HAL_TIMER_setTimeCmpCount(TIMERBASE, 500, 1);
}

int main(void)
{
	HAL_GPIO_setRegPinDirection(LEDGPIO, 0xFF);

	HAL_TIMER_setTimeCmpCount(TIMERBASE, configTICK_RATE_HZ, 1);
	HAL_TIMER_setCallbackFunction(handle_timer2, 1);
	
	HAL_TIMER_enableInterrupt(TIMERBASE, 1);

	main_blinky();
}

void ToggleLED( void )
{
    HAL_GPIO_togglePinOutput(LEDGPIO, LED1);
}
