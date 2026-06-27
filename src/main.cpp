#include <Arduino.h>
#include "AudioPlayer.h"
#include "Countdown.h"
#include "Display.h"
#include "Keypad.h"

AudioPlayer player(16, 17); // DFPlayer Mini object
Countdown countdown; // Countdown object
Display display(5, 18); // Display object
Keypad keypad(32, 33, 25, 26, 27, 14, 12, 13); // Keypad object

// Create state machine to handle the different states of the program
enum State
{
  SETTING_TIME,
  COUNTING_DOWN,
  PAUSED,
  ALARM,
  TASK
};

// Initial state
State currentState = SETTING_TIME;

// Variable to store the time being set
int startTime = 0;

void setup() 
{
  // Initializations
  Serial.begin(115200); // Initialize the baud rate for debugging
  display.init();
  player.init();
  keypad.init();
}

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
      display.displayNumber(startTime);
      keypad.updateTime(key, startTime);
      keypad.updateEvents(key);

      // Check if the enter button is pressed to change states
      if(keypad.enterPressed()) 
      {
        // Check that 00:00 has not been entered
        if(startTime == 0) break;

        else
        {
          // Make sure the time being set is valid
          countdown.modifyTime(startTime);

          // Reset the timer to begin counting down, then change states
          countdown.resetLastDecrement();
          currentState = COUNTING_DOWN;
        }
      }

      // If pause is pressed, clear it in this state
      if(keypad.pausePressed()) {}

      // If the reset button is pressed, reset the time being set to 0
      else if(keypad.resetPressed()) startTime = 0;

      break;
    }

    case COUNTING_DOWN:
    {
      // Get and return the pressed key
      uint8_t key = keypad.getKey();

      // Update the events
      keypad.updateEvents(key);

      // Begin counting down while displaying the time
      display.displayNumber(startTime);
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
      display.displayNumber(startTime);

      // Check if the resume button is pressed to change states
      if(keypad.resumePressed()) 
      {
        // Reset the timer to begin counting down, then change states
        countdown.resetLastDecrement();
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
      display.displayNumber(0);

      // Play the alarm
      player.playAlarm();

      // Get and return the pressed key
      uint8_t key = keypad.getKey();

      // Update the events
      keypad.updateEvents(key);

      // Check if the enter key has been entered
      if(keypad.enterPressed())
      {
        // Switch to the interactive state
        currentState = TASK;
      }

      break;
    }

    case TASK:
    {
      // Display 100 as a test
      display.displayNumber(100);
      player.test();
    }
  }
}