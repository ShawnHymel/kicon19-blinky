/**
 * Basic fade blinky for ATtiny85
 * Author: Shawn Hymel
 * Date: April 6, 2019
 * 
 * Add board manager: 
 * https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json
 * 
 * Install "attiny" board definitions.
 * 
 * Settings:
 *  Board: ATtiny25/45/85
 *  Processor: ATtiny85
 *  Clock: Internal 1 MHz
 * 
 * Beerware!
 */

#include <avr/sleep.h>
#include <avr/wdt.h>

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
                                      128, 151, 180, 214, 248}; 

// Interrupt Service Routine (Watchdog Timer)
ISR(WDT_vect) {

  // Disable Watchdog Timer
  wdt_reset();                        // Reset watchdog
  MCUSR &= ~(1 << WDRF);              // Make sure flag is cleared
  WDTCR |= (1 << WDCE) | (1 << WDE);  // Special operation to let us disable WDT
  WDTCR = 0x00;                       // Turn off WDT*/
}

// Pulse LED slowly on and off once
void pulseLED() {
  
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
}

void setup() {

  // Configure pins
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);

  // Disable ADC (must be before writing to PRR or ADC will be stuck on)
  ADCSRA = 0;

  // Disable power to peripherals via PRR (Timer1, USI, ADC)
  PRR = (1 << PRTIM1) | (1 << PRUSI) | (1 << PRADC);

}

void loop() {

  // Do the thing
  pulseLED();

  // Configure Watchdog Timer
  uint8_t wdt_timeout = (1 << WDP2) | (1 << WDP1) | (1 << WDP0); // 1 s timeout
  cli();                              // Disable interrupts
  wdt_reset();                        // Reset watchdog
  MCUSR = 0;                          // Clear reset flags
  WDTCR = (1 << WDCE) | (1 << WDE);   // Special operation to change prescaler
  WDTCR = (1 << WDIE) | wdt_timeout;  // Enable interrupts, set timeout
  sei();                              // Enable interrupts
  
  // Sleep sequence
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // Only INT0, USI, and WDT can wake
  cli();                                // Disable interrupts
  sleep_enable();                       // Set Sleep Enable bit in MCUCR
  sei();                                // Enable interrupts
  sleep_cpu();                          // Go to sleep
  
  // -> Wake up here <-

  // Disable sleeping
  sleep_disable();     // Clear Sleep Enable bit in MCUCR
}
