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
    //Initialize UART connection
    uart_init(UART_BAUD_SELECT(9600, F_CPU));

    // Configure Analog-to-Digital Convertion unit
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"
    ADMUX = ADMUX | (1<<REFS0);
    // Select input channel ADC0 (voltage divider pin)
    ADMUX = ADMUX & ~(1<<MUX3 | 1<<MUX2 | 1<<MUX1 | 1<<MUX0);
    // Enable ADC module
    ADCSRA = ADCSRA | (1<<ADEN);
    // Enable conversion complete interrupt
    ADCSRA = ADCSRA | (1<<ADIE);
    // Set clock prescaler to 128
    ADCSRA = ADCSRA | (1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0);
    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 33 ms and enable overflow interrupt
    TIM1_overflow_262ms();
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
 * Purpose:  Use rotative encoder and detect when switch to ADC converter.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
  //Variables to read values of the rotative encoder
  static uint8_t lastStateA = 0;
  uint8_t newStateA = GPIO_read(&PIND, PD2); 
  static uint8_t counter = 0;
  char string[4];
  static char password[4] = "1234";
  //Variables to read values of the push button of the rotative encoder
  static uint8_t pushButton = 0;
  //static uint8_t joystick_press = 0;
  //Get a counter of push button of the rotative encoder
  if (GPIO_read(&PIND, PD1) == 0) pushButton++;
  //Display on the lcd the value of the counter of rotation
  //at the position according to the value of pushButton (x-axis)
  if(pushButton < 4){
    lcd_gotoxy(pushButton,0);
    itoa(counter,string,10);
    lcd_puts(string);
  }
  //Else we put the push button counter at 0
  else{
    lcd_clrscr();
    pushButton = 0;
  }

  //If we turn the rotative encoder
  if(newStateA != lastStateA){
    //If values are not the same we increment
    if (GPIO_read(&PIND,PD3) != newStateA && counter < 9) counter++;
    //Or decrement the counter
    else if(counter > 0) counter--;
  }
  //Put the last value at the new value
  lastStateA = newStateA;
  
  

  //If the push button of the joystick is pressed
  //if (!GPIO_read(&DDRB, PB2) || joystick_press){
    // Start ADC conversion
    //ADCSRA = ADCSRA | (1<<ADSC);
  //}
  /*
 
  ADCSRA = ADCSRA | (1<<ADSC);
  uint16_t value;
  char string[4];
  static uint8_t line = 0;
  static uint8_t counter = 0;
  static uint8_t lastStateA = 0;
  uint8_t newStateA = GPIO_read(&PIND, PD2);

  value = ADC;

  if(value > 1000 && line == 1) line = 0;
  else if(value < 10 && line == 0) line = 1;

  if(!GPIO_read(&PIND,PD1)) counter++;
  
  if(newStateA != lastStateA){
    //If values are not the same we increment
    if (GPIO_read(&PIND,PD3) != newStateA && counter < 9) counter++;
    //Or decrement the counter
    else if(counter > 0) counter--;
  }
  //Put the last value at the new value
  lastStateA = newStateA;

  itoa(counter,string,10);
  lcd_puts(string);*/
  

}

