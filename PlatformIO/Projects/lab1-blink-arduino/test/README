# Lab 1: Ryan VERNEL

### Morse code

1. Listing of C code which repeats one "dot" and one "comma" (BTW, in Morse code it is letter `A`) on a LED. Always use syntax highlighting, meaningful comments, and follow C guidelines:

```c
int main(void)
{
    // Set pin where on-board LED is connected as output
    #define LED_RED   PB0
    #define PB0 8

    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_RED, HIGH);

    // Infinite loop
    while (1)
    {
        digitalWrite(LED_RED, LOW);   //The led is ON because it is in active low

        _delay_ms(100); //The led stay ON during 100 milliseconds to do the "dot"

        digitalWrite(LED_RED, HIGH);  //The led in now OFF after a delay of 100 milliseconds

        _delay_ms(200);

        digitalWrite(LED_RED, LOW);   //Turn again the led ON after 500 milliseconds

        _delay_ms(500); //The led stay ON during 500 milliseconds to do the "comma"

        digitalWrite(LED_RED, HIGH);

        _delay_ms(200);

    }
    
    // Will never reach this
    return 0;
}
```

2. Scheme of Morse code application, i.e. connection of AVR device, LED, resistor, and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![Scheme of Morse code application](https://raw.githubusercontent.com/Rayou01/digitals-electronics-2/main/PlatformIO/Projects/lab1-blink-arduino/test/Scheme%20of%20Morse%20code%20application.png)
