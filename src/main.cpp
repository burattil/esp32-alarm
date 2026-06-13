#include <TM1637Display.h>
#include "Countdown.h"
#include "Display.h"
#include "Keypad.h"

Keypad keypad(13, 12, 14, 27, 26, 25, 33, 32);

// Create state machine to handle the different states of the program
enum State
{
  SETTING_TIME,
  COUNTING_DOWN,
  ALARM
};

// Initial state
State currentState = SETTING_TIME;

// Variable to store the time being set
int startTime = 0;

void setup() 
{
  // Initializations
  keypad.init();
  displayInit();
}

void loop() 
{
  // Switch statement derived from the state machine
  switch(currentState)
  {
    // Get the initial value from the user
    case SETTING_TIME:
    {
      // Continuously display the time being set and update the time
      displayNumber(startTime);
      keypad.update(startTime);

      // Check if the enter button is pressed to change states
      if(keypad.enterPressed()) currentState = COUNTING_DOWN;

      break;
    }

    case COUNTING_DOWN:
    {
      displayNumber(7777); // TEST VALUE TO SHOW IT HAS CHANGED STATES

      break;
    }

    case ALARM:
    {
      break;
    }
  }
}