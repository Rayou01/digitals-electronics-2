# Project made by : RYAN VERNEL and MATHIS NICKELS

## Introduction
This project consist in make an application using a LCD, a joystick and a rotative encoder.
So for this project we need tu use:

* *LCD screen*
* *Rotative encoder*
* *Joystick*
* *LED*

## Team Members
* __Ryan Vernel__ responsible for __programming__
* __Mathis Nickels__ responsible for __schematic__

## The goal of the project
The goal is 2 little games. One of them, consist in found the good password using the rotative encoder device
and the other game is like the hangman game using joystick device

So, let see the use of each device:

* __LCD screen__ allows to display information and games
* __Rotative encoder__ allows to play at the first game (find the password):
    * __Turn__: scroll through the numbers
    * __Push__: select the number / change game
* __Joystick__ allows to play at the second game (hangman game):
    * __x-axis__: scroll through the letters
    * __push__: select the letter / change game
* __LED__ is turn on when you win a game

Here, you can see the structure of our project. As you can see, we don't need the test repository so we deleted it.
```c
PROJECT                             // PlatfomIO project
├── include                         // Included files
├── lib                             // Libraries
│    │── gpio                       //GPIO library
│    │     │── gpio.c
│    │     └── gpio.h
│    │── lcd                        //LCD library
│    │     │── lcd_definitions.h
│    │     │── lcd.c
│    │     └── lcd.h
├── src                             // Source file
│   └── main.c
├── platformio.ini                  // Project Configuration File
└── README.md                       // Report of this project
```

### Hardware description

## Pin connection for LCD
header j1 Bottom half
   | **Pin** | **Signal** | **Arduino Pin** | **Description** |
   | :-: | :-: | :-: | :-: |
   | 7 | DB4 | PD4 | Data bit 4 |
   | 8 | DB5 | PD5 | Data bit 5 |
   | 9 | DB6 | PD6 | Data bit 6 |
   | 10 | DB7 | PD7 | Data bit 7 |
   | 11 | GND | GND | GND |
   | 12 | VCC | 5V | Power Supply of Arduino |

header j2
   | **Pin** | **Signal** | **Arduino Pin** | **Description** |
   | :-: | :-: | :-: | :-: |
   | 1 | RS | PB0 | Register Select: High for Data Transfer, Low for Instruction Transfer |
   | 2 | R/W | GND | Read/Write signal: High for Read mode, Low for Write mode  |
   | 3 | E | PB1 | Read/Write Enable: High for Read, falling edge writes data  |
   | 4 | NC |  | Optional back-light enable (not connected on the PmodCLP) |
   | 5 | GND |  | GND |
   | 6 | VCC |  | Power Supply of Arduino |

## Pin connection for Joystick
   | **Signal** | **Arduino Pin** | **Description** |
   | :-: | :-: | :-: |
   | VRx | PD4 | Voltage Proportional to X axis |
   | VRy | PD5 | Voltage Proportional to Y axis |
   | SW | PD6 | Push button |
   | GND | GND | GND |
   | VCC | 5V | Power Supply of Arduino |

## Pin connection for rotary encoder
   | **Signal** | **Arduino Pin** | **Description** |
   | :-: | :-: | :-: |
   | CLK (output A) | PD3 | is the primary output pulse to determine the amount of rotation. Each time the knob is turned in either direction by just one detent (click), the ‘CLK’ output goes through one cycle of going HIGH and then LOW. |
   | DT (output B) | PD2 |  is similar to CLK output, but it lags behind CLK by a 90° phase shift. This output is used to determine the direction of rotation. |
   | SW | PB3 | is the active low push button. When the knob is pressed down, the state goes LOW. |
   | GND | GND | GND |
   | VCC | 5V | Power Supply of Arduino |

## Pin connection for LED in active low
| **Signal** | **Arduino Pin** |
   | :-: | :-: |
   | Anode | 5V |
   | Cathode | PB4 |

## Main structure of the project
First, the structure of the project can be devided in 2 parts: the main part and then the interrupt part with games code.

Here the code for the main structure of the code that we will detail for you:
```c
/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions
```
In this part we include some libraries needed for the code.

Then, we create some variables for 1st and 2nd game outside the main function:
```c
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
```
In this case, we can use this variables in any functions because they are __global variables__.

### Main function
Now, we have the main function of the project.
```c
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

    //Set LED in output pin and OFF
    GPIO_mode_output(&DDRB, PB4);
    GPIO_write_high(&PORTB,PB4);

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
```
As you can see, we initialize 
* variables, 
* LCD, 
* ouput pin for the LED, 
* and we set ADC convertion and TIM1 OVF.

To make it easier, you can see the flowchart of main function that explains you what do the function:

  ![flowchart of main function](https://raw.githubusercontent.com/Rayou01/digitals-electronics-2/main/PlatformIO/Projects/project/flowchart_main-function.png)

## Structure or the 2nd part: Interrupts
Now for the second part of the code, there is interrupts function. It's here where games code will be executed.

For TIM1_OVF, we just start the ADC conversion every 33ms as you can see below:
```c
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
```


```c

```

  ![flowchart of TIM1_OVF]()

The we have the ADC_vect executed every 33ms when the ADC conversion is started. This interrupt is the main interrupt of games code.

## Global structure of the project