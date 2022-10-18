| **LCD signal(s)** | **AVR pin(s)** | **Description** |
   | :-: | :-: | :-: |
   | RS | PB0  | Register selection signal. Selection between Instruction register (RS=0) and Data register (RS=1) |
   | R/W | GND | Identifies read (R/W=1) or write (R/W=0) operation. If R/W=GND : the LCD could only receiving 
                 instructions/data and the is no able read the status from the LCD |
   | E | PB1  | falling edge of E enables (starts) the communication. Falling edge of E enables (starts) the communication |
   | D[3:0] | -- | Data bus line |
   | D[7:4] | PD[7:4]  | Data bus line |
   | K | PB2/GND ?  | Backlight cathode |


   ![Picture of ASCII table](https://raw.githubusercontent.com/Rayou01/digitals-electronics-2/main/PlatformIO/Projects/lab4-lcd/test/Pre-Lab/ASCII-Table.png)