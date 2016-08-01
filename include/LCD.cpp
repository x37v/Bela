#include <stdio.h>
#include <string.h>

#include "LCD.h"

LCD::LCD(){}

void LCD::setup(BelaContext *context, unsigned int rs, unsigned int enable,
			 unsigned int d0, unsigned int d1, unsigned int d2, unsigned int d3,
			 unsigned int d4, unsigned int d5, unsigned int d6, unsigned int d7)
{
    _rs_pin = rs;
    // _rw_pin - this isn't defined because it's connected to ground. This means it's always
    // in write mode and we save on a digital pin!
    _enable_pin = enable;
  
    _data_pins[0] = d0;
    pinMode(context, 0, d0, OUTPUT);
    _data_pins[1] = d1;
    pinMode(context, 0, d1, OUTPUT);
    _data_pins[2] = d2;
    pinMode(context, 0, d2, OUTPUT);
    _data_pins[3] = d3; 
    pinMode(context, 0, d3, OUTPUT);
    _data_pins[4] = d4;
    pinMode(context, 0, d4, OUTPUT);
    _data_pins[5] = d5;
    pinMode(context, 0, d5, OUTPUT);
    _data_pins[6] = d6;
    pinMode(context, 0, d6, OUTPUT);
    _data_pins[7] = d7; // Most significant
    pinMode(context, 0, d7, OUTPUT);
  
    pinMode(context, 0, _rs_pin, OUTPUT);
    // // we can save 1 pin by not using RW. Indicate by passing 255 instead of pin#

    pinMode(context, 0, _enable_pin, OUTPUT);
  
    rt_printf("Setup successful\n");
  
  
}

void LCD::begin(BelaContext *context, unsigned int rows, unsigned int cols) {
    _numrows = rows;
    _numcols = cols;
    _charsize =  LCD_5x8DOTS;
    // WE NEED TO WAIT FOR 50ms HERE!! How is that going to work? Count samples from audio?
    
    // // Set the RS and E pins low to get ready:
    digitalWrite(context, 0, _rs_pin, LOW);
    digitalWrite(context, 0, _enable_pin, LOW);
    
    rt_printf("Begin successful\n");
    
}

void LCD::command(unsigned int value) {
  send(context, value, 0);
}


// write() sends a single char:
int LCD::write(unsigned int value) {
  send(context, value, 1);
  
  return 1; // assume sucess
}

void LCD::send(BelaContext *context, unsigned int value, unsigned int mode) {
    digitalWrite(context, 0, _rs_pin, value);

    // write8bits(value); 
    
}

void LCD::write8bits(unsigned int byte) {
    
}
