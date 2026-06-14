#include <Arduino.h>
#include "Display.h"

#define CLK 5
#define DIO 18

TM1637Display display(CLK, DIO);

// Function to initialize the display
void displayInit()
{
  // Set the brightness, display zeroes, and enable the colon
  display.setBrightness(7);
  display.showNumberDecEx(0000, 0b01000000, true);

  return;
}

// Function to display a number
void displayNumber(int num)
{
  display.showNumberDecEx(num, 0b01000000, true);

  return;
}

// Helper function to change the seconds into minutes and seconds
int minuteAndSecondDecrementer(int* x) 
{
  // Count down in terms of minutes and seconds
  if(*x == 0) *x = 0;                                 // If it is 0, keep it at 0
  else if(*x % 100 == 0) *x -= 41;                    // If it is about to turn to 99, switch it to 59
  else (*x)--;                                        // If it is a regular number, decrement it by 1

  return *x;
}