#ifndef DISPLAY_H
#define DISPLAY_H

#include <TM1637Display.h>

extern TM1637Display display;

// Function prototypes
void displayInit();
void displayNumber(int num);
void countdown(int* num);

#endif