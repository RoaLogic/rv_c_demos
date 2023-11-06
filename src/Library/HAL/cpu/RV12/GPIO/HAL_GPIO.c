////////////////////////////////////////////////////////////////////
//   ,------.                    ,--.                ,--.         //
//   |  .--. ' ,---.  ,--,--.    |  |    ,---. ,---. `--' ,---.   //
//   |  '--'.'| .-. |' ,-.  |    |  |   | .-. | .-. |,--.| .--'   //
//   |  |\  \ ' '-' '\ '-'  |    |  '--.' '-' ' '-' ||  |\ `--.   //
//   `--' '--' `---'  `--`--'    `-----' `---' `-   /`--' `---'   //
//                                             `---'              //
//                                                                //
//      HAL_GPIO source file                                      //
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

#include "HAL_GPIO.h"

/** @brief Set the register mode for all pins
 * @details Function sets the given pin setting into the mode register of the given base address.
 *
 * @note This function does not check the previous value of the register, it just writes the given setting.
 *
 * @param baseAddress	The base address pointer for the GPIO
 * @param pinSetting	The setting to place in the mode register
 */
void HAL_GPIO_setRegPinMode(HAL_GPIO_Type* baseAddress, uint8_t pinSetting)
{
	baseAddress->mode = pinSetting;
}

/** @brief Set the register direction for all pins
 * @details Function sets the given pin setting into the direction register of the given base address.
 *
 * @note This function does not check the previous value of the register, it just writes the given setting.
 *
 * @param baseAddress	The base address pointer for the GPIO
 * @param pinSetting	The setting to place in the mode register
 */
void HAL_GPIO_setRegPinDirection(HAL_GPIO_Type* baseAddress, uint8_t pinSetting)
{
	baseAddress->direction = pinSetting;
}

/** @brief Set the mode for the given pin
 * @details Function to set a pin into a given mode
 *
 * @param baseAddress	The base address pointer for the GPIO
 * @param pinNum		The pin number to set
 * @param pinSetting	The setting for the given pin
 */
void HAL_GPIO_setPinMode(HAL_GPIO_Type* baseAddress, uint8_t pinNum, uint8_t pinSetting)
{
   if (pinNum < PIN_NUM_MAX)
   {
	   if(pinSetting == PUSH_PULL)
	   {
		   baseAddress->mode &= ~(1 << pinNum);
	   }
	   else
	   {
		   baseAddress->mode  |= (1 << pinNum);
	   }
   }
}

/** @brief Set the mode for the given pin
 * @details Function to set a pin into a given mode
 *
 * @param baseAddress	The base address pointer for the GPIO
 * @param pinNum		The pin number to set
 * @param pinSetting	The setting for the given pin
 */
void HAL_GPIO_setPinDirection(HAL_GPIO_Type* baseAddress, uint8_t pinNum, uint8_t pinSetting)
{
   if (pinNum < PIN_NUM_MAX)
   {
       if(pinSetting == INPUT)
       {
       	   baseAddress->direction &= ~(1 << pinNum);
       }
       else
       {
       	   baseAddress->direction  |= (1 << pinNum);
       }
   }
}

/** @brief Enable the pin interrupt for the given pin
 *
 * @param baseAddress	The base address pointer for the GPIO
 * @param pinNum		The pin number to enable
 */
void HAL_GPIO_enablePinInterrupt(HAL_GPIO_Type* baseAddress, uint8_t pinNum)
{
   if (pinNum < PIN_NUM_MAX)
   {
	   baseAddress->irqEnable |= (1 << pinNum );
   }
}

/** @brief Disable the pin interrupt for the given pin
 *
 * @param baseAddress	The base address pointer for the GPIO
 * @param pinNum		The pin number to disable
 */
void HAL_GPIO_disablePinInterrupt(HAL_GPIO_Type* baseAddress, uint8_t pinNum)
{
   if (pinNum < PIN_NUM_MAX)
   {
	   baseAddress->irqEnable &= ~(1 << pinNum );
   }
}

/** @brief Set the pin interrupt setting
 * @details Function to set the pin interrupt setting
 *
 * It can setup the GPIO module to generate an interrupt in different settings.
 * The trigger type, level triggered or edge triggered can be selected, as well
 * on which level or which edge to trigger on.
 *
 * By default the setting is off
 *
 * Possible settings:
 * 	Trigger type: level triggered (0) or edge triggered (1)
 * 	Trigger setting level: Not triggered (0), High level (1) or low level (2)
 * 	Trigger setting edge: Not triggered (0), falling edge (1),
 * 						  rising edge (2) or both edges (3)
 *
 * @param baseAddress	 The base address pointer for the GPIO
 * @param pinNum		 The pin number to enable
 * @param triggerType	 The trigger type to set
 * @param triggerSetting The trigger setting to use
 */
void HAL_GPIO_setPinInterrupt(HAL_GPIO_Type* baseAddress, uint8_t pinNum, uint8_t triggerType, uint8_t triggerSetting)
{
   if (pinNum < PIN_NUM_MAX)
   {
	   if(triggerType == LEVEL_TRIGGERED)
	   {
		   baseAddress->triggerType &= ~( 1 << pinNum );

		   if(triggerSetting == HIGH_LEVEL_TRIGGERED)
		   {
			   baseAddress->triggerLvl0 &= ~( 1 << pinNum );
			   baseAddress->triggerLvl1 |= ( 1 << pinNum );
		   }
		   else if(triggerSetting == LOW_LEVEL_TRIGGERED)
		   {
			   baseAddress->triggerLvl0 |= ( 1 << pinNum );
			   baseAddress->triggerLvl1 &= ~( 1 << pinNum );
		   }
		   else
		   {
			   baseAddress->triggerLvl0 &= ~( 1 << pinNum );
			   baseAddress->triggerLvl1 &= ~( 1 << pinNum );
		   }
	   }
	   else
	   {
		   baseAddress->triggerType |= ( 1 << pinNum );

		   if(triggerSetting == FALLING_EDGE_TRIGGERED)
		   {
			   baseAddress->triggerLvl0 |= ( 1 << pinNum );
			   baseAddress->triggerLvl1 &= ~( 1 << pinNum );
		   }
		   else if(triggerSetting == RISING_EDGE_TRIGGERED)
		   {
			   baseAddress->triggerLvl0 &= ~( 1 << pinNum );
			   baseAddress->triggerLvl1 |= ( 1 << pinNum );
		   }
		   else if(triggerSetting == BOTH_EDGE_TRIGGERED)
		   {
			   baseAddress->triggerLvl0 |= ( 1 << pinNum );
			   baseAddress->triggerLvl1 |= ( 1 << pinNum );
		   }
		   else
		   {
			   baseAddress->triggerLvl0 &= ~( 1 << pinNum );
			   baseAddress->triggerLvl1 &= ~( 1 << pinNum );
		   }
	   }
   }
}

/** @brief Return the current trigger status
 *
 * @param baseAddress	 The base address pointer for the GPIO
 * @return	The trigger status
 */
uint8_t HAL_GPIO_getTriggerStatus(HAL_GPIO_Type* baseAddress)
{
	return baseAddress->triggerStatus;
}

/** @brief Set the register output for all pins
 * @details Function sets the given pin setting into the output register of the given base address.
 *
 * @note This function does not check the previous value of the register, it just writes the given setting.
 *
 * @param baseAddress	The base address pointer for the GPIO
 * @param pinSetting	The setting to place in the output register
 */
void HAL_GPIO_setRegPinOutput(HAL_GPIO_Type* baseAddress, uint8_t pinSetting)
{
	baseAddress->output = pinSetting;
}

/** @brief Set the pin output
 * @details Function to set the given pin output state
 *
 * @param baseAddress	 The base address pointer for the GPIO
 * @param pinNum		 The pin number to set
 * @param pinSetting	 The setting to set the pin too, 1 for high, 0 for low
 */
void HAL_GPIO_setPinOutput(HAL_GPIO_Type* baseAddress, uint8_t pinNum, uint8_t pinSetting)
{
   if (pinNum < PIN_NUM_MAX)
   {
	   if(pinSetting == 0)
	   {
		   baseAddress->output &= ~(1 << pinNum);
	   }
	   else
	   {
		   baseAddress->output  |= (1 << pinNum);
	   }
   }
}

/** @brief Toggle the pin output for the given pin number
 *
 * @param baseAddress	 The base address pointer for the GPIO
 * @param pinNum		 The pin number to toggle
 */
void HAL_GPIO_togglePinOutput(HAL_GPIO_Type* baseAddress, uint8_t pinNum)
{
	baseAddress->output ^= (1 << pinNum);
}

/** @brief Get the register from the pin input
 * @details Function gets the input value of the given GPIO base address
 *
 *
 * @param baseAddress	The base address pointer for the GPIO
 * @return The input pin setting for the given base GPIO
 */
uint8_t HAL_GPIO_getRegPinInput(HAL_GPIO_Type* baseAddress)
{
	return baseAddress->input;
}

/** @brief Get the pin input
 * @details Function to get the state of a input pin
 *
 * @param baseAddress	The base address pointer for the GPIO
 * @param pinNum		 The pin number to enable
 * @return the state of the pin, 0 or 1
 */
uint8_t HAL_GPIO_getPinInput(HAL_GPIO_Type* baseAddress, uint8_t pinNum)
{
	uint8_t returnValue = 0;

	if(pinNum < PIN_NUM_MAX)
	{
		returnValue = (baseAddress->input & (1 << pinNum)) >> pinNum;
	}

	return returnValue;
}

