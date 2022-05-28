/* 
 ### RaveBOX v1.0 ###
                               
 Super simple, one knob synthesizer and sequencer
 Copyright (C) 2020-2022, Vladimir Bartos
 License: CC BY-NC-SA https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/

#include <avr/io.h>
#include <util/delay.h>

// Pin definitions
#define LED       PB1
#define SPK       PB0
#define BUT1      PB3
#define BUT2      PB4
#define POT       A1

uint8_t mem[161];
uint8_t seq;
uint8_t a;


void setup() {

  // Setup pins
  DDRB |= (1 << LED) | (1 << SPK);           // Set output pins
  DDRB &= (~(1 << BUT1) | ~(1 << BUT2));     // Set input pins
  PORTB |= (1 << BUT1) | (1 << BUT2);        // Pull-up for buttons

  // Setup Timer0
  TCCR0A = ((1 << WGM01) | (1 << COM0A0));   // CTC Mode, Toggle Output A
  TCCR0B = (1 << CS02);                      // Prescale 256

  // Setup ADC
  analogReference(DEFAULT);
}


void loop() {

  for (uint8_t seq = 0; seq <= 160; seq++) {

    // Save Tone
    if (digitalRead(BUT1) == 0) {
      mem[seq] = 255 - analogRead(POT) / 4;
    }

    // Save Kick
    if (digitalRead(BUT2) == 0 || a > 1 ) {
      if (a < 60) {
        a = 60;
      }
      a = a + 15;
      mem[seq] = a;
      if (a == 240) {
        a = 0;
      }
    }

    // Reset Pattern
    if (digitalRead(BUT1) == 0 && digitalRead(BUT2) == 0) {
      DDRB &= ~(1 << SPK);
      for (uint8_t i = 0; i <= 160; i++) {
        mem[i] = 0;
      }
      _delay_ms(800);
      a = 0;
    }

    // Blink LED
    if (seq == 1 || seq == 39 || seq == 79 || seq == 119) {
      PORTB |= (1 << LED);
    }
    if (seq == 10 || seq == 49 || seq == 89 || seq == 129) {
      PORTB &= ~(1 << LED);
    }

    // Play pattern
    if (mem[seq] > 0) {
      DDRB |= (1 << SPK);
    }
    else {
      DDRB &= ~(1 << SPK);
    }

    OCR0A = mem[seq];
    _delay_ms(15);

  }

}
