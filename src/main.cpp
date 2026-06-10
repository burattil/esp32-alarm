#include <TM1637Display.h>
#include "Display.h"
#include "Keypad.h"

Keypad keypad(13, 12, 14, 27, 26, 25, 33, 32);

int num;

void setup() {
  // put your setup code here, to run once:
  keypad.init();
  displayInit();
  num = keypad.getAndDisplayInput();
}

void loop() {
  // put your main code here, to run repeatedly:
  
}