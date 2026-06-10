#include <TM1637Display.h>
#include "Display.h"
#include "Keypad.h"

Keypad keypad(13, 12, 14, 27, 26, 25, 33, 32);

int num;

void setup() {
  // Initializations
  keypad.init();
  displayInit();

  // CHANGE KEYPAD FROM HANDLING DISPLAY
  num = keypad.getAndDisplayInput();
}

void loop() {
  
}