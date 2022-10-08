/*****************Leds with push button program *****************/

/*
 * Define all PINs
 */
#define BP    PD2
#define LED_1 PD4
#define LED_2 PD5
#define LED_3 PD6
#define LED_4 PD7
#define LED_5 PB0

#define PD2 2
#define PD4 4
#define PD5 5
#define PD6 6
#define PD7 7
#define PB0 8

/*
 * Creation of variables
 */
int state_BP = 1;
int read_BP;
int previous_state_BP = 1;

/*
 * Function that put all LEDs in off
 */
void leds_off()
{
  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, HIGH);
  digitalWrite(LED_3, HIGH);
  digitalWrite(LED_4, HIGH);
  digitalWrite(LED_5, HIGH);
}//leds_off()

void setup() {
  /*
   * Define of output PINs
   */
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(LED_5, OUTPUT);
  /*
   * Define the push button PIN that is an 
   * input pullup that means it contains
   * an internal resistor
   */
  pinMode(BP, INPUT_PULLUP);
  /*
   * Put all LEDs off at the beginning
   */
  leds_off(); //function created
}//setup()

void loop() {
  /*
   * Read the value of the button
   */
  read_BP = digitalRead(BP);

  /*
   * Detection of the rising edge
   */
  if(read_BP != previous_state_BP){
    previous_state_BP = read_BP;
    if (read_BP == 1) {
      state_BP = !state_BP; //change the state of the button
    }//if
  }//if

  //if the button is equal to 0 :
  if(state_BP == 0){ 
    digitalWrite(LED_5, HIGH);
    digitalWrite(LED_1, LOW);
    delay(100);
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, LOW);
    delay(100);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, LOW);
    delay(100);
    digitalWrite(LED_3, HIGH);
    digitalWrite(LED_4, LOW);
    delay(100);
    digitalWrite(LED_4, HIGH);
    digitalWrite(LED_5, LOW);
    delay(100);
  }//if
  else leds_off(); //function created
}//loop()

/* Here there is the link to the schematic for this application : 
 * https://raw.githubusercontent.com/Rayou01/digitals-electronics-2/main/PlatformIO/Projects/lab2-gpio_library/test/Extra_Exercices/lab2_LEDs_and_push_button.png
 */
