#ifndef DISPLAY_H
#define DISPLAY_H

#include <TM1637Display.h>

// Create the display object
class Display
{
  private:
    // Create the object that is used privately
    TM1637Display display;

  public:
    // Constructor function
    Display(uint8_t clk, uint8_t dio);

    // Function to initialize the display
    void init();

    // Function to display a number
    void displayNumber(int num);
};

#endif