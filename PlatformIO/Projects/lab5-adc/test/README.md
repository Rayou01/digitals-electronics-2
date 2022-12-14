# Lab 5: Ryan VERNEL

### Analog-to-Digital Conversion

1. Complete table with voltage divider, calculated, and measured ADC values for all five push buttons.

   | **Push button** | **PC0[A0] voltage** | **ADC value (calculated)** | **ADC value (measured)** | **ADC value (measured, hex)** |
   | :-: | :-: | :-: | :-: | :-: |
   | Right  | 0 V | 0 | 0 | 0 |
   | Up     | 0.495 V | 101 | 100 | 64 |
   | Down   | 1.203 V | 246 | 257 | 101 |
   | Left   | 1.97 V | 403 | 410 | 19A |
   | Select | 3.182 V | 651 | 641 | 281 |
   | none   | 5 V | 1023 | 1023 | 3FF |

### Temperature meter

Consider an application for temperature measurement. Use analog temperature sensor [TC1046](http://ww1.microchip.com/downloads/en/DeviceDoc/21496C.pdf), LCD, and a LED. Every 30 seconds, the temperature is measured and the value is displayed on LCD screen. When the temperature is too high, the LED will turn on.

2. Draw a schematic of temperature meter. The image can be drawn on a computer or by hand. Always name all components and their values.

   ![temperature measurement application](https://raw.githubusercontent.com/Rayou01/digitals-electronics-2/main/PlatformIO/Projects/lab5-adc/test/temperature%20measurement%20application.png)

3. Draw two flowcharts for interrupt handler `TIMER1_OVF_vect` (which overflows every 1 sec) and `ADC_vect`. The image can be drawn on a computer or by hand. Use clear descriptions of the individual steps of the algorithms.

   ![flowchart-lab5-Part1](https://raw.githubusercontent.com/Rayou01/digitals-electronics-2/main/PlatformIO/Projects/lab5-adc/test/flowchart-lab5-part1.png)
   ![flowchart-lab5-Part2](https://raw.githubusercontent.com/Rayou01/digitals-electronics-2/main/PlatformIO/Projects/lab5-adc/test/flowchart-lab5-part2.png)
   ![flowchart-lab5-Part3](https://raw.githubusercontent.com/Rayou01/digitals-electronics-2/main/PlatformIO/Projects/lab5-adc/test/flowchart-lab5-part3.png)
