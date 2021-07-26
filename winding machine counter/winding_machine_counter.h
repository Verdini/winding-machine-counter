
#ifndef contador_bobinadeira_h
#define contador_bobinadeira_h

#define VERSION 2.3.6


// Definitions for device customization

// Define LCD Address
//#define LCD_ADDRESS 0x3F
#define LCD_ADDRESS 0x27

// Define LCD Splash Screen message (max 16 chars per line)
#define LCD_SPLASH_LINE_0 "Inicializando o "
#define LCD_SPLASH_LINE_1 "sistema..."


// Libraries used by software
#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// Fast IO read/write
// The default arduino functions to read the pins are too slow. We need faster functions.
// Solution source: http://masteringarduino.blogspot.com/2013/10/fastest-and-smallest-digitalread-and.html
// Arduino I/O functions replacements:
// pinMode( pin, INPUT ); -> pinAsInput( pin );
// pinMode( pin, OUTPUT ); -> pinAsOutput( pin );
// pinMode( pin, INPUT_PULLUP);	-> pinAsInputPullUp( pin );
// digitalWrite( pin, LOW ); -> digitalLow( pin );
// digitalWrite( pin, HIGH ); -> digitalHigh( pin );
// digitalRead( pin ) -> digitalState( pin )		

#define portOfPin(P)\
  (((P)>=0&&(P)<8)?&PORTD:(((P)>7&&(P)<14)?&PORTB:&PORTC))
#define ddrOfPin(P)\
  (((P)>=0&&(P)<8)?&DDRD:(((P)>7&&(P)<14)?&DDRB:&DDRC))
#define pinOfPin(P)\
  (((P)>=0&&(P)<8)?&PIND:(((P)>7&&(P)<14)?&PINB:&PINC))
#define pinIndex(P)((uint8_t)(P>13?P-14:P&7))
#define pinMask(P)((uint8_t)(1<<pinIndex(P)))

#define pinAsInput(P) *(ddrOfPin(P))&= ~pinMask(P)
#define pinAsInputPullUp(P) *(ddrOfPin(P))&= ~pinMask(P); digitalHigh(P)
#define pinAsOutput(P) *(ddrOfPin(P))|= pinMask(P)
#define digitalLow(P) *(portOfPin(P))&= ~pinMask(P)
#define digitalHigh(P) *(portOfPin(P))|= pinMask(P)
#define isHigh(P)((*(pinOfPin(P)) & pinMask(P))>0)
#define isLow(P)((*(pinOfPin(P)) & pinMask(P)) == 0)
#define digitalState(P)((uint8_t)isHigh(P))


// Arduino pin definitions
#define ENCODER_INTERRUPT 0
#define PIN_ENCODER_A 2
#define PIN_ENCODER_B 3
#define PIN_OUT_1 13
#define PIN_OUT_2 9


// Encoder Pins: A (Orange) - D2, B (Yellow) - D3 
// LCD Pins: SDA - A4, SCL - A5
// Button Pins: R: D4, P: D5, Up: 6, Left: 7
// Out Pins: SP1: D8, SP2: D9; to test, use SP1: 13 (built in led)


// Files used by program. Do not alter order
#include "settings.h"
#include "display.h"
#include "buttons.h"
#include "program.h"


#endif
