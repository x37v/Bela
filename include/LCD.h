/*

This is a library for printing to an LCD display with Bela.
This library was developed for the Hitachi HD44780 chipset (and compatible variants).

Right now this library only works in 8 bit mode.

Roadmap:
- 4 bit mode to save on digital GPIO
- Different segment sizes (currently only 5x8)

By Astrid Bin
August 2016

*/

#ifndef LCD_h
#define LCD_h

#include <Bela.h>
#define LCD_5x8DOTS 0x00

class LCD {

public:
    LCD();
  
    void setup(BelaContext *context, unsigned int rs, unsigned int enable,
	    unsigned int d0, unsigned int d1, unsigned int d2, unsigned int d3,
	    unsigned int d4, unsigned int d5, unsigned int d6, unsigned int d7);
    
    void begin(BelaContext *context, unsigned int rows, unsigned int cols);
    
    void clear();
    void home();
    void noDisplay();
    void blink();
    void noCursor();
    void cursor();
    // Set the row and col of where you want the cursor to be:
    void setCursor(unsigned int, unsigned int);
    
    void command(unsigned int);
    void send(BelaContext *context, unsigned int, unsigned int);
    int write(unsigned int);
    void write8bits(unsigned int);

private:
 
    BelaContext *context;
    unsigned int _rs_pin; // LOW: command.  HIGH: character.
    unsigned int _enable_pin; // activated by a HIGH pulse.
    unsigned int _data_pins[8];
    unsigned int _numrows;
    unsigned int _numcols;
    unsigned int _charsize;

    bool _ready;

};

#endif
