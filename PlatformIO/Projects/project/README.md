# Project made by : RYAN VERNEL and MATHIS NICKELS

## Introduction
This project consist in make an application using a LCD, a joystick and a rotative encoder.
So for this project we need tu use:

* LCD screen
* Rotative encoder
* Joystick
* UART (maybe RTC)
* LED

## Team Members
* __Ryan Vernel__ responsible for __programming__
* __Mathis Nickels__ responsible for __schematic__

## The goal of the project
The goal is 2 little games. One of them, consist in found the good password using the rotative encoder device
and the other game is like the hangman game using joystick device

So, let see the use of each device:
* LCD screen allows to display information and games
* Rotative encoder allows to play at the first game (find the password):
* Turn: scroll through the number 
    `* Push: select the number`
* Joystick allows to play at the second game (hangman game):
    `* x-axis: change the position of the cursor`
    `* y-axis: scroll the letters`
    `* push: permits to change the game between both`
* UART allows ti print informations like Level1, You have 2 good answers...
* LED is turn on when you win a game

OUR_PROJECT                        // PlatfomIO project
├── include                         // Included files
├── lib                             // Libraries
│    │── gpio                       //GPIO library
│    │     │── gpio.c
│    │     └── gpio.h
│    │── lcd                        //LCD library
│    │     │── lcd_definitions.h
│    │     │── lcd.c
│    │     └── lcd.h
│    └── uart                       //UART library
│         │── uart.c
│         └── lcd.h
├── src                             // Source file(s)
│   └── main.c
├── platformio.ini                  // Project Configuration File
└── README.md                       // Report of this project