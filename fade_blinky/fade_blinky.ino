/**
 * Basic fade blinky for ATtiny85
 * Author: Shawn Hymel
 * Date: April 6, 2019
 * 
 * Beerware!
 */

const int LED0 = 0;
const int LED1 = 1;

const int WAIT_TIME = 10;
const int OFF_TIME = 1000;

void setup() {
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
}

void loop() {
  for ( int i = 0; i <= 255; i++ ) {
    analogWrite(LED0, i);
    analogWrite(LED1, i);
    delay(WAIT_TIME);
  }
  
  for ( int i = 255; i >= 0; i-- ) {
    analogWrite(LED0, i);
    analogWrite(LED1, i);
    delay(WAIT_TIME);
  }

  delay(OFF_TIME);
}
