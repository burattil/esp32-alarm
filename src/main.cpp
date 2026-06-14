#include <TM1637Display.h>
#include "Countdown.h"
#include "Display.h"
#include "Keypad.h"

Countdown countdown; 
Keypad keypad(13, 12, 14, 27, 26, 25, 33, 32);

// Create state machine to handle the different states of the program
enum State
{
  SETTING_TIME,
  COUNTING_DOWN,
  PAUSED,
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
// UPDATE KEYPAD.UPDATE STARTTIME THING AS YOU KNOW
void loop() 
{
  // Switch statement derived from the state machine
  switch(currentState)
  {
    // Get the initial value from the user
    case SETTING_TIME:
    {
      // Get and return the pressed key
      uint8_t key = keypad.getKey();

      // Continuously display the time being set and update the time and events
      displayNumber(startTime);
      keypad.updateTime(key, startTime);
      keypad.updateEvents(key);

      // Check if the enter button is pressed to change states
      if(keypad.enterPressed()) 
      {
        // Make sure the time being set is valid
        countdown.modifyTime(startTime);

        // Reset the timer to begin counting down, then change states
        countdown.resetTimer();
        currentState = COUNTING_DOWN;
      }

      break;
    }

    case COUNTING_DOWN:
    {
      // Get and return the pressed key
      uint8_t key = keypad.getKey();

      // Update the events
      keypad.updateEvents(key);

      // Begin counting down while displaying the time
      displayNumber(startTime);
      countdown.decrementTimeMMSS(startTime);

      // Check if the pause button is pressed to change states
      if(keypad.pausePressed()) currentState = PAUSED;

      // Check if the reset button is pressed to change states
      else if(keypad.resetPressed()) 
      {
        currentState = SETTING_TIME;

        // Reset the time being set to 0
        startTime = 0;
      }

      // Check if the time has reached 0 to change states
      else if(startTime == 0) currentState = ALARM;

      break;
    }

    case PAUSED:
    {
      // Get and return the pressed key
      uint8_t key = keypad.getKey();

      // Update the events
      keypad.updateEvents(key);

      // Continue displaying the time while paused
      displayNumber(startTime);

      // Check if the resume button is pressed to change states
      if(keypad.resumePressed()) 
      {
        // Reset the timer to begin counting down, then change states
        countdown.resetTimer();
        currentState = COUNTING_DOWN;
      }

      // Check if the reset button is pressed to change states
      else if(keypad.resetPressed()) 
      {
        currentState = SETTING_TIME;

        // Reset the time being set to 0
        startTime = 0;
      }

      break;
    }

    case ALARM:
    {
      // Display 0 to indicate that the timer has been completed
      displayNumber(0);

      break;
    }
  }
}