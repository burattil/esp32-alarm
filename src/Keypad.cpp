#include <Arduino.h>
#include "Keypad.h"

// Save the row and column pins to the class to be used throughout the function 
Keypad::Keypad(uint8_t rA, uint8_t rB, uint8_t rC, uint8_t rD, uint8_t cA, uint8_t cB, uint8_t cC, uint8_t cD)
{
  rOne = rA;
  rTwo = rB;
  rThree = rC;
  rFour = rD;

  cOne = cA;
  cTwo = cB;
  cThree = cC;
  cFour = cD;
}

// Initalize the rows and columns as outputs and inputs (with pull-up resistors), respectively
void Keypad::init()
{
  pinMode(rOne, OUTPUT);
  pinMode(rTwo, OUTPUT);
  pinMode(rThree, OUTPUT);
  pinMode(rFour, OUTPUT);
  pinMode(cOne, INPUT_PULLUP);
  pinMode(cTwo, INPUT_PULLUP);
  pinMode(cThree, INPUT_PULLUP);
  pinMode(cFour, INPUT_PULLUP);

  return;
}
 
// Scan once here
uint8_t Keypad::scanOnce()
{
  // Drive the first row low, others high
    digitalWrite(rOne, LOW);
    digitalWrite(rTwo, HIGH);
    digitalWrite(rThree, HIGH);
    digitalWrite(rFour, HIGH);

    // Now check which column is low, and return the associated value with that
    if(digitalRead(cOne) == LOW) return 1;
    if(digitalRead(cTwo) == LOW) return 2;
    if(digitalRead(cThree) == LOW) return 3;
    if(digitalRead(cFour) == LOW) return 10;

    // Drive the second row low, others high
    digitalWrite(rOne, HIGH);
    digitalWrite(rTwo, LOW);
    digitalWrite(rThree, HIGH);
    digitalWrite(rFour, HIGH);

    // Now check which column is low, and return the associated value with that
    if(digitalRead(cOne) == LOW) return 4;
    if(digitalRead(cTwo) == LOW) return 5;
    if(digitalRead(cThree) == LOW) return 6;
    if(digitalRead(cFour) == LOW) return NO_KEY;

    // Drive the third row low, others high
    digitalWrite(rOne, HIGH);
    digitalWrite(rTwo, HIGH);
    digitalWrite(rThree, LOW);
    digitalWrite(rFour, HIGH);

    // Now check which column is low, and return the associated value with that
    if(digitalRead(cOne) == LOW) return 7;
    if(digitalRead(cTwo) == LOW) return 8;
    if(digitalRead(cThree) == LOW) return 9;
    if(digitalRead(cFour) == LOW) return NO_KEY;

    // Drive the fourth row low, others high
    digitalWrite(rOne, HIGH);
    digitalWrite(rTwo, HIGH);
    digitalWrite(rThree, HIGH);
    digitalWrite(rFour, LOW);

    // Now check which column is low, and return the associated value with that
    if(digitalRead(cOne) == LOW) return NO_KEY;
    if(digitalRead(cTwo) == LOW) return 0;
    if(digitalRead(cThree) == LOW) return NO_KEY;
    if(digitalRead(cFour) == LOW) return 13;

    // If nothing is pressed, return the no key constant
    return NO_KEY;
}

// Scan the rows and return the associated button——this might end up being a helper function
uint8_t Keypad::getKey()
{
  // Create the variable to store which key is pressed
  uint8_t key = scanOnce();

  // If a button has been detected
  if(key != NO_KEY && lastKey == NO_KEY)
  {
    // Add a non-blocking debounce
    if(millis() - lastPress > 20) 
    {
      // Update the time of last press, last press, and return
      lastPress = millis();
      lastKey = key;
      return key;
    }
  }

  // If nothing is detected
  else if(key == NO_KEY)
  {
    lastKey = NO_KEY;
  }

  return NO_KEY;
}

// Function to update the global variable for the time being set
void Keypad::update(int& time)
{
  // Get the key being pressed
  uint8_t key = getKey();

  // Update if something other than NO_KEY is detected from the scan
  if(key != NO_KEY)
  {
    // If 'A' is pressed, delete the last digit
    if(key == 10) time /= 10;

    // If 'D' is pressed, set the enter variable
    else if(key == 13) enterEvent = true;

    // If any other number is pressed
    else if(time / 1000 == 0) time = time * 10 + key;
  }

  return;
}

// Function to determine if the enter button ('D') is pressed to change states
bool Keypad::enterPressed()
{
  // If 'D' is pressed, return true
  if(enterEvent) 
  {
    // Reset the enter event variable and return true
    enterEvent = false;
    return true;
  }

  // Otherwise, return false
  return false;
}