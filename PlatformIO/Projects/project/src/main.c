/***********************************************************************
 * 
 * Use Analog-to-digital conversion to read push buttons on LCD keypad
 * shield and display it on LCD screen.
 * 
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2018 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <uart.h>
#include <stdlib.h>         // C library. Needed for number conversions


/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and start ADC conversion every 100 ms.
 *           When AD conversion ends, send converted value to LCD screen.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Initialize display
    lcd_init(LCD_DISP_ON);

    uint8_t valA;
    uint8_t counter = 0;
    valA = GPIO_read(&DDRD,PD3);
    //Initialize UART connection
    uart_init(UART_BAUD_SELECT(9600, F_CPU));

    // Configure Analog-to-Digital Convertion unit
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"
    //ADMUX = ADMUX | (1<<REFS0);
    // Select input channel ADC0 (voltage divider pin)
    //ADMUX = ADMUX & ~(1<<MUX3 | 1<<MUX2 | 1<<MUX1 | 1<<MUX0);
    // Enable ADC module
    //ADCSRA = ADCSRA | (1<<ADEN);
    // Enable conversion complete interrupt
    //ADCSRA = ADCSRA | (1<<ADIE);
    // Set clock prescaler to 128
    //ADCSRA = ADCSRA | (1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0);
    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 33 ms and enable overflow interrupt
    TIM1_overflow_33ms();
    TIM1_overflow_interrupt_enable();
    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}


/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Use single conversion mode and start conversion every 100 ms.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
  uint8_t valA;
  uint8_t counter = 0;
  uint8_t new_valA = GPIO_read(&DDRD,PD3);
  if(valA != new_valA){
    if(GPIO_read(&DDRD,PD2) != new_valA){
      counter++;
    }
    else{
      counter--;
    }
  }
  valA = new_valA;



  /*
  uint8_t value = 0;
  char string[4];
  value = GPIO_read(&DDRD,PD3);
  itoa(value,string,10);
  uart_puts(string);
  uart_puts("\n\r");*/
  
  
  
    // Start ADC conversion
    //ADCSRA = ADCSRA | (1<<ADSC);
}

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display converted value on LCD screen.
 **********************************************************************/
ISR(ADC_vect)
{
    uint16_t value;
    static uint8_t no_of_overflows = 0;
    char string[4];  // String for converted numbers by itoa()

    // Read converted value
    // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC
    no_of_overflows++;
    if(no_of_overflows >= 6){
      no_of_overflows = 0;

      value = ADC;
      // Convert "value" to "string" and display it
      itoa(value, string, 10);
      uart_puts(string);
      uart_puts("\n\r");
    }   
}