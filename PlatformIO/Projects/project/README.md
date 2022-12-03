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