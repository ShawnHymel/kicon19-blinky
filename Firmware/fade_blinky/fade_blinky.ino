/**
 * Basic fade blinky for ATtiny85
 * Author: Shawn Hymel
 * Date: April 6, 2019
 * 
 * Beerware!
 */

const int LED0 = 0;
const int LED1 = 1;

const int WAIT_TIME = 50;     // Time (ms) between LED updated
const int OFF_TIME = 1000;    // Time (ms) LED is off

// Perceived linear fade scale
const uint8_t PWM_TABLE_SIZE = 30;
const uint8_t pwm_table[PWM_TABLE_SIZE] PROGMEM = { 
                                        0,   1,   2,   3,   4,
                                        5,   6,   7,   8,   9,
                                       10,  11,  14,  16,  19,  
                                       23,  27,  32,  38,  45,  
                                       54,  64,  76,  90, 107, 
                                      128, 151, 180, 214, 255}; 

void setup() {
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
}

void loop() {

  // Fade on
  for ( int8_t i = 0; i < PWM_TABLE_SIZE; i++ ) {
    analogWrite(LED0, pgm_read_byte(&pwm_table[i]));
    analogWrite(LED1, pgm_read_byte(&pwm_table[i]));
    delay(WAIT_TIME);
  }

  // Fade off
  for ( int8_t i = PWM_TABLE_SIZE - 1; i >= 0; i-- ) {
    analogWrite(LED0, pgm_read_byte(&pwm_table[i]));
    analogWrite(LED1, pgm_read_byte(&pwm_table[i]));
    delay(WAIT_TIME);
  }

  delay(OFF_TIME);
}
