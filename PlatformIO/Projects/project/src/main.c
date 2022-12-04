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
 * Purpose:  Use Timer/Counter1 and start ADC conversion every 33 ms.
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
    TIM1_overflow_33ms();
    TIM1_overflow_interrupt_enable();
    // Enables interrupts by setting the global interrupt mask
    sei();

    //Initialization of variables
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

    //Display the beggining of the first game
    for (uint8_t i = 0; i < 4; i++){
      lcd_gotoxy(i,0);
      itoa(input_numbers[i],string,10);
      lcd_puts(string);
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
    // Start ADC conversion
    ADCSRA = ADCSRA | (1<<ADSC);
}

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display games on LCD screen which can be played with rotative encoder and joystick.
 **********************************************************************/
ISR(ADC_vect)
{
  //allows us to know when switch beetwen both games
  static uint8_t code = 0;
  //if the joystick have been pushed 
  //(or if it's the first time) 
  if (code == 0){
    //variables for rotary encoder
    uint8_t newStateA = GPIO_read(&PIND,PD3);
    uint8_t newStatePushButton = GPIO_read(&PINB,PB2);

    //If we press the button of the encoder, we take the value choosen
    //and we compare it with the value to find
    if (lastStatePushButton != newStatePushButton && !newStatePushButton && lastStatePushButton){
      input_numbers[pushButton_encoder] = counter;
      if (input_numbers[pushButton_encoder] == numbers[pushButton_encoder] && pushButton_encoder <= 4) result_password++;
      pushButton_encoder++;
    }
    //change state push button of rotary encoder
    lastStatePushButton = newStatePushButton;

    //if we turn the rotary encoder
    if (lastStateA != newStateA){
      //If we turn in a specific way we increment
      if (GPIO_read(&PIND,PD2) != newStateA){
        if (counter < 9) counter++;
      }
      //else we decrement
      else if (counter > 0) counter--;
    }
    //Put the last value at the new value
    lastStateA = newStateA;

    //if we press the button to restart
    if (pushButton_encoder == 5){
      lcd_clrscr();
      GPIO_write_high(&PORTB,PB4);
      counter = 0;
      result_password = 0;
      input_numbers[0] = 0;
      input_numbers[1] = 0;
      input_numbers[2] = 0;
      input_numbers[3] = 0;
      lcd_gotoxy(0,0);
      pushButton_encoder = 0;
      //print default number to play again
      for (uint8_t i = 0; i < 4; i++){
        lcd_gotoxy(i,0);
        itoa(input_numbers[i],string,10);
        lcd_puts(string);
      }
    }
    //When we play at the game
    else if (pushButton_encoder < 4){
      lcd_gotoxy(pushButton_encoder,0);
      itoa(counter,string,10);
      lcd_puts(string);
    }
    else{
      //if we find all numbers
      if(result_password == 4){
        GPIO_write_low(&PORTB,PB4);
        lcd_gotoxy(5,0);
        lcd_puts("You win");
        lcd_gotoxy(0,1);
        lcd_puts("Press to restart");
      }
      //if we loose
      else{
        lcd_gotoxy(5,0);
        lcd_puts("You loose");
        lcd_gotoxy(0,1);
        lcd_puts("Press to restart");
      }
    }
    //if we press the push button of the joystick
    if (!GPIO_read(&PINB,PB3)){
      code = 1;
      lcd_clrscr();
      lcd_gotoxy(0,1);
      itoa(nbr_life,string,10);
      lcd_puts(string);

      //Print custom caracter on many pixels
      for (uint8_t j = 0; j < 2; j++){
        lcd_gotoxy(j+4,0);
        for (uint8_t i = 0; i < 8; i++)
          lcd_data(customChar[i]);
        lcd_command(1<<LCD_DDRAM);
      }
      //we wait until the button is pushed 
      //to not play the first number at the same time
      while (!GPIO_read(&PINB,PB3)){}
    }
  }

  //if the joystick have been pushed
  else{
    //variables for joystick
    uint16_t newStateX = ADC;
    static uint8_t letter = 0;
    static uint8_t counter_letter_found = 0;
    uint8_t newStatePBJoystick = GPIO_read(&PINB,PB3);
  
    //if we push on x axis the joystick
      //at the top: increment
      //at the bottom: decrement
    if (newStateX > 1000 && lastStateX < 1000){
      if(letter < 25) letter++;
      else if (letter == 25) letter = 0;
    }
    else if (newStateX < 20 && lastStateX > 20){
      if(letter > 0) letter--;
      else if (letter == 0) letter = 25;
    }
    //change state of x axis
    lastStateX = newStateX;

    //if the game is finished
    if (counter_letter_found == 7 || nbr_life == 0){
      letter = 0;

      //if we loose
      if (nbr_life == 0){
        lcd_gotoxy(3,0);
        lcd_puts("You loose");
        lcd_gotoxy(0,1);
        lcd_puts("Press to restart");
      }
      //if we win
      else{
        GPIO_write_low(&PORTB,PB4);
        lcd_gotoxy(4,0);
        lcd_puts("You win");
        lcd_gotoxy(0,1);
        lcd_puts("Press to restart");
      }
      //if we press push button we restart the game
      //we restart the game
      if (lastStatePBJoystick != newStatePBJoystick && !newStatePBJoystick && lastStatePBJoystick){
        lcd_clrscr();
        counter_letter_found = 0;
        nbr_life = 10;
        GPIO_write_high(&PORTB,PB4);

        for (uint8_t j = 0; j < 2; j++){
          lcd_gotoxy(j+4,0);
          for (uint8_t i = 0; i < 8; i++)
            lcd_data(customChar[i]);
          lcd_command(1<<LCD_DDRAM);
        }
        //While we press the push button of joystick we wait
        while (!GPIO_read(&PINB,PB3)){}
        //to not restart and select a letter at the same time
        return;
      }
    }
    //when we play
    else if (nbr_life > 0){
      GPIO_write_high(&PORTB,PB4);
      lcd_gotoxy(7,1);
      lcd_putc(letters[letter]);
      lcd_gotoxy(0,1);
      itoa(nbr_life,string,10);
      if (nbr_life < 10) lcd_puts("0");
      lcd_puts(string);
    }
    //if we push the button of the joystick
    //we compare each letter of the word with the selectionnated letter
    if (lastStatePBJoystick != newStatePBJoystick && !newStatePBJoystick && lastStatePBJoystick){
        uint8_t compare = 0;

        for (uint8_t i = 0; i < 7; i++){
          if (word[i] == letters[letter]){
            counter_letter_found++;
            lcd_gotoxy(i+4,0);
            lcd_putc(letters[letter]);
            compare++;
          }
        }
        //if the letter is not in the word, we loose a life
        if (compare == 0) nbr_life--;
    }
    //change state of push button of the joystick
    lastStatePBJoystick = newStatePBJoystick;

    //if we push the push button of the encoder
    //we switch to the other game
    if (!GPIO_read(&PINB,PB2)){
      code = 0;
      lcd_clrscr();
      GPIO_write_high(&PORTB,PB4);
      counter = 0;
      result_password = 0;
      input_numbers[0] = 0;
      input_numbers[1] = 0;
      input_numbers[2] = 0;
      input_numbers[3] = 0;
      lcd_gotoxy(0,0);
      pushButton_encoder = 0;
      for (uint8_t i = 0; i < 4; i++){
        lcd_gotoxy(i,0);
        itoa(input_numbers[i],string,10);
        lcd_puts(string);
        //we wait until the button is pushed 
        //to not play the first number at the same time
        while (!GPIO_read(&PINB,PB2)){}
      }
    }
  }
}