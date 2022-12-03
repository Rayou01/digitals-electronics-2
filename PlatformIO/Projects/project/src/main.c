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
#include <stdlib.h>         // C library. Needed for number conversions

//For game with the joystick
uint8_t lastStatePBJoystick;
uint16_t lastStateX;
char word[7];
char letters[26];
uint8_t nbr_life;

//For game with the encoder
uint8_t lastStateA;
uint8_t lastStatePushButton;
uint8_t pushButton_encoder;
uint8_t input_numbers[4];
uint8_t result_password;
uint8_t counter;
uint8_t numbers[4];


char string[4]; //To convert number in string (via itoa)
uint8_t customChar[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b11111
};

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

    // Set addressing to CGRAM (Character Generator RAM)
    lcd_command(1<<LCD_CGRAM);

    GPIO_mode_output(&DDRB, PB4);
    GPIO_write_high(&PORTB,PB4);

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

    lastStateA = GPIO_read(&PIND,PD3);
    lastStatePushButton = GPIO_read(&PINB,PB2);
    pushButton_encoder = 0;
    result_password = 0;
    counter = 0;


    input_numbers[0] = 0;
    input_numbers[1] = 0;
    input_numbers[2] = 0;
    input_numbers[3] = 0;

    numbers[0] = 5;
    numbers[1] = 4;
    numbers[2] = 8;
    numbers[3] = 1;

    for (uint8_t i = 0; i < 4; i++){
      lcd_gotoxy(i,0);
      itoa(input_numbers[i],string,10);
      lcd_puts(string);
    }

    lastStatePBJoystick = GPIO_read(&PINB,PB3);
    lastStateX = 512;
    nbr_life = 10;

    word[0] = 'D';
    word[1] = 'I';
    word[2] = 'G';
    word[3] = 'I';
    word[4] = 'T';
    word[5] = 'A';
    word[6] = 'L';

    for (uint8_t i = 0; i < 26; i++){
      letters[i] = 'A' + i;
    }

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
 * Purpose:  Use single conversion mode and start conversion every 33 ms.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
  //Variables to read values of the rotative encoder
  static uint8_t lastStateA = 0;
  uint8_t newStateA = GPIO_read(&DDRD, PD2);
  static uint8_t counter = 0;
  char string[4];
  //Variables to read values of the push button of the rotative encoder
  static uint8_t pushButton = 0;
  //static uint8_t joystick_press = 0;
  
  //Get a counter of push button of the rotative encoder
  if (GPIO_read(&DDRD, PD1) == 1){
    pushButton++;
    itoa(pushButton, string, 10);
    lcd_puts(string);
    lcd_gotoxy(0,0);
  }
  //Display on the lcd the value of the counter of rotation
  //at the position according to the value of pushButton (x-axis)
  //if(pushButton < 4){
    //lcd_gotoxy(pushButton,0);
    //lcd_putc(counter);
  //}
  //Else we put the push button counter at 0
  //else{
    //lcd_clrscr();
    //pushButton = 0;
  //}

  //If we turn the rotative encoder
  if(newStateA != lastStateA){
    //If values are not the same we increment
    if (lastStateA != newStateA && counter < 9) counter++;
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
}

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Use joystick and detect when rotative encoder is pressed.
 **********************************************************************/
ISR(ADC_vect)
{
    uint16_t value;
    char string[4];  // String for converted numbers by itoa()
    uint8_t no_of_overflows = 0;
    no_of_overflows++;
    if(no_of_overflows >= 6){
      no_of_overflows = 0;
      // Read converted value
      // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC
      value = ADC;
      // Convert "value" to "string" and display it
      itoa(value, string, 10);
      uart_puts(string);
      uart_puts("\n\r");
    }   
}

