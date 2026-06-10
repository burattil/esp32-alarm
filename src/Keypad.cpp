#include "Display.h"
#include "Keypad.h"
#include <Arduino.h>

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
    if(digitalRead(cFour) == LOW) return 0;

    // Drive the third row low, others high
    digitalWrite(rOne, HIGH);
    digitalWrite(rTwo, HIGH);
    digitalWrite(rThree, LOW);
    digitalWrite(rFour, HIGH);

    // Now check which column is low, and return the associated value with that
    if(digitalRead(cOne) == LOW) return 7;
    if(digitalRead(cTwo) == LOW) return 8;
    if(digitalRead(cThree) == LOW) return 9;
    if(digitalRead(cFour) == LOW) return 0;

    // Drive the first row low, others high
    digitalWrite(rOne, HIGH);
    digitalWrite(rTwo, HIGH);
    digitalWrite(rThree, HIGH);
    digitalWrite(rFour, LOW);

    // Now check which column is low, and return the associated value with that
    if(digitalRead(cOne) == LOW) return 0;
    if(digitalRead(cTwo) == LOW) return 0;
    if(digitalRead(cThree) == LOW) return 0;
    if(digitalRead(cFour) == LOW) return 13;

    // If nothing is pressed, return the no key constant
    return NO_KEY;
}

// Scan the rows and return the associated button——this might end up being a helper function
uint8_t Keypad::getKey()
{
  // Create the variable to store which key is pressed
  uint8_t key;

  // Continue looping until one of the return statements is hit
  while(1)
  {
    key = scanOnce();

    // If a button has been detected
    if(key != NO_KEY)
    {
      // Debounce
      delay(20);

      // Check that the same key is being pressed
      if(scanOnce() == key)
      {
        // Wait for the release
        while(scanOnce() != NO_KEY)
        {
          delay(1);
        }

        return key;
      }
    }
  }
}

// Function to get the input while displaying it along the way
int Keypad::getAndDisplayInput()
{
  // Create an integer to store the pressed value in, and the final value
  uint8_t value = 0;
  int inputValue = 0;

  // Display the value as it comes
  displayNumber(0);

  // Continue until the enter button ('D') is pressed
  while(1)
  {
    // Get a value
    value = getKey();

    // "Subtract" if the delete button ('A') has been pressed
    if(value == 10) inputValue /= 10;

    // Exit the function and return the number if the enter button ('D') is pressed
    else if(value == 13)
    {
      // Make sure that the number is within the appropriate range——correct it if needed
      if(inputValue >= 9959) inputValue = 9959;
      else if(inputValue == 0) inputValue = 1;

      // Return the value of num after this
      return inputValue;
    }

    // "Add" the other number iff num is not already four digits
    else if(inputValue / 1000 == 0) inputValue = inputValue * 10 + value;

    // Update the display 
    displayNumber(inputValue);
  }
}