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
    if(digitalRead(cFour) == LOW) return DELETE_KEY;

    // Drive the second row low, others high
    digitalWrite(rOne, HIGH);
    digitalWrite(rTwo, LOW);
    digitalWrite(rThree, HIGH);
    digitalWrite(rFour, HIGH);

    // Now check which column is low, and return the associated value with that
    if(digitalRead(cOne) == LOW) return 4;
    if(digitalRead(cTwo) == LOW) return 5;
    if(digitalRead(cThree) == LOW) return 6;
    if(digitalRead(cFour) == LOW) return RESET_KEY;

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
    if(digitalRead(cOne) == LOW) return PAUSE_KEY;
    if(digitalRead(cTwo) == LOW) return 0;
    if(digitalRead(cThree) == LOW) return RESUME_KEY;
    if(digitalRead(cFour) == LOW) return ENTER_KEY;

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
    if(millis() - lastPress > DEBOUNCE_TIME) 
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
void Keypad::updateTime(uint8_t key, int& time)
{
  // Update if something other than NO_KEY, ENTER, PAUSE, or RESUME is detected from the scan
  if(key != NO_KEY && key != ENTER_KEY && key != PAUSE_KEY && key != RESUME_KEY && key != RESET_KEY)
  {
    // If DELETE is pressed, delete the last digit
    if(key == DELETE_KEY) time /= 10;

    // If any other number is pressed
    else if(time / 1000 == 0 ) time = time * 10 + key;
  }

  return;
}

// Function to update the events
void Keypad::updateEvents(uint8_t key)
{
  // If ENTER is pressed, set the enter variable
  if(key == ENTER_KEY) enterEvent = true;

  // If RESET is pressed, reset the reset variable
  else if(key == RESET_KEY) resetEvent = true;

  // If PAUSE is pressed, set the pause variable
  else if(key == PAUSE_KEY) pauseEvent = true;

  // If RESUME is pressed, set the resume variable
  else if(key == RESUME_KEY) resumeEvent = true;

  return;
}

// Function to determine if ENTER is pressed to change states
bool Keypad::enterPressed()
{
  // If ENTER is pressed, return true
  if(enterEvent) 
  {
    // Reset the enter event variable and return true
    enterEvent = false;
    return true;
  }

  // Otherwise, return false
  return false;
}

// Function to determine if PAUSE is pressed to change states
bool Keypad::pausePressed()
{
  // If PAUSE is pressed, return true
  if(pauseEvent) 
  {
    // Reset the pause event variable and return true
    pauseEvent = false;
    return true;
  }

  // Otherwise, return false
  return false;
}

// Function to determine if RESUME is pressed to change states
bool Keypad::resumePressed()
{
  // If RESUME is pressed, return true
  if(resumeEvent) 
  {
    // Reset the resume event variable and return true
    resumeEvent = false;
    return true;
  }

  // Otherwise, return false
  return false;
}

// Function to determine if RESET is pressed to change states
bool Keypad::resetPressed()
{
  // If RESET is pressed, return true
  if(resetEvent) 
  {
    // Reset the reset event variable and return true
    resetEvent = false;
    return true;
  }

  // Otherwise, return false
  return false;
}