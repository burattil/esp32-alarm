#include "Display.h"

// Constructor function
Display::Display(uint8_t clk, uint8_t dio)
  // Create the object
  : display(clk, dio) {}

  // Initializing function
void Display::init()
{
  // Set the brightness, display zeroes, and enable the colon
  display.setBrightness(7);
  display.showNumberDecEx(0000, 0b01000000, true);

  return;
}

// Function to display a number
void Display::displayNumber(int num)
{
  display.showNumberDecEx(num, 0b01000000, true);

  return;
}