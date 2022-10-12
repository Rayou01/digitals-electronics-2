
 | **Module** | **Number of bits** | **1** | **8** | **32** | **64** | **128** | **256** | **1024** |
   | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-- |
   | Timer/Counter0 | 8 | 16u | 128u | -- | 1024u | -- | 4096u | 16384u |
   | Timer/Counter1 | 16 | 4096u | 32768u | -- | 0,26s | -- | 1,05s | 4,19s |
   | Timer/Counter2 | 8 | 16u | 128u | 512u | 1024u | 2048u | 4096u | 16384u |


# Lab 2: Ryan VERNEL

### GPIO control registers

1. Complete the table for DDRB and PORTB control register values.

   | **DDRB** | **PORTB** | **Direction** | **Internal pull-up resistor** | **Description** |
   | :-: | :-: | :-: | :-: | :-- |
   | 0 | 0 | input | no | Tri-state, high-impedance |
   | 0 | 1 | input | yes | PB will source current if ext. pulled low |
   | 1 | 0 | output | no | Output Low |
   | 1 | 1 | output | no | Output High |

### GPIO library

2. Complete the table with code sizes from three versions of the blink application.

   | **Version** | **Size [B]** |
   | :-- | :-: |
   | Arduino-style     | 480 |
   | Registers         | 146 |
   | Library functions | 158 |

### Traffic light

3. Scheme of traffic light application with one red/yellow/green light for cars, one red/green light for pedestrians, and one push button. Connect AVR device, LEDs, resistors, push button (for pedestrians), and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![Scheme of traffic light application](https://raw.githubusercontent.com/Rayou01/digitals-electronics-2/main/PlatformIO/Projects/lab2-gpio_library/test/Scheme%20of%20traffic%20light%20application.png)
