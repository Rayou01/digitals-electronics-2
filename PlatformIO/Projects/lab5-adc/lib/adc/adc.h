#ifndef ADC_H
# define ADC_H

/***********************************************************************
 * 
 * ADC library for AVR-GCC.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2022 Ryan Vernel
 * This work is licensed under the terms of the MIT license.
 *
 **********************************************************************/

/**
 * @file 
 * @defgroup vernel_adc ADC Library <adc.h>
 * @code #include "adc.h" @endcode
 *
 * @brief adc library for AVR-GCC.
 *
 * @note Based on AVR Libc Reference Manual. Tested on ATmega328P 
 *       (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2.
 * @author Ryan Vernel
 * @copyright (c) 2019 Tomas Fryza, This work is licensed under 
 *                the terms of the MIT license
 * @{
 */


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>
#include <lcd.h>


/* Function prototypes -----------------------------------------------*/
/**
 * @brief  Configure one output pin.
 * @param  s String to display on the lcd
 * @param  x horizontal position\n (0: left most position)
 * @param  y vertical position\n   (0: first line)
 * @return none
 */
void ADC_print_button_name(const char *s, uint8_t x, uint8_t y);

/** @} */

#endif