#include <Arduino.h>
#include "AudioPlayer.h"
#include "Countdown.h"
#include "Display.h"
#include "Keypad.h"
#include "Task.h"

// Constants for the DFPlayer Mini
#define RX_PIN 16
#define TX_PIN 17
#define BUSY_PIN 23

// Constants for the TM1637Display
#define CLK 5
#define DIO 18

// Constants for the 4x4 Keypad
#define ROW_ONE 32
#define ROW_TWO 33
#define ROW_THREE 25
#define ROW_FOUR 26
#define COL_ONE 27
#define COL_TWO 14
#define COL_THREE 12
#define COL_FOUR 13

AudioPlayer audioPlayer(RX_PIN, TX_PIN, BUSY_PIN); // DFPlayer Mini object
Countdown countdown; // Countdown object
Display display(CLK, DIO); // Display object
Keypad keypad(ROW_ONE, ROW_TWO, ROW_THREE, ROW_FOUR, COL_ONE, COL_TWO, COL_THREE, COL_FOUR); // Keypad object
Task task(audioPlayer); // Task object

// Create state machine to handle the different states of the program
enum class MainState
{
  SETTING_TIME,
  COUNTING_DOWN,
  PAUSED,
  ALARM,
  TASK
};

// Initial state
MainState mainState = MainState::SETTING_TIME;

// Variable to store the time being set
int startTime = 0;

void setup() 
{
  // Initializations
  Serial.begin(115200); // Initialize the baud rate for debugging
  display.init();
  audioPlayer.init();
  keypad.init();
}

void loop() 
{
  // Switch statement derived from the state machine
  switch(mainState)
  {
    // Get the initial value from the user
    case MainState::SETTING_TIME:
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
          mainState = MainState::COUNTING_DOWN;
        }
      }

      // If pause is pressed, clear it in this state
      if(keypad.pausePressed()) {}

      // If the reset button is pressed, reset the time being set to 0
      else if(keypad.resetPressed()) startTime = 0;

      break;
    }

    case MainState::COUNTING_DOWN:
    {
      // Get and return the pressed key
      uint8_t key = keypad.getKey();

      // Update the events
      keypad.updateEvents(key);

      // Begin counting down while displaying the time
      display.displayNumber(startTime);
      countdown.decrementTimeMMSS(startTime);

      // Check if the pause button is pressed to change states
      if(keypad.pausePressed()) mainState = MainState::PAUSED;

      // Check if the reset button is pressed to change states
      else if(keypad.resetPressed()) 
      {
        mainState = MainState::SETTING_TIME;

        // Reset the time being set to 0
        startTime = 0;
      }

      // Check if the time has reached 0 to change states
      else if(startTime == 0) mainState = MainState::ALARM;

      break;
    }

    case MainState::PAUSED:
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
        mainState = MainState::COUNTING_DOWN;
      }

      // Check if the reset button is pressed to change states
      else if(keypad.resetPressed()) 
      {
        mainState = MainState::SETTING_TIME;

        // Reset the time being set to 0
        startTime = 0;
      }

      break;
    }

    case MainState::ALARM:
    {
      // Display 0 to indicate that the timer has been completed
      display.displayNumber(0);

      // Play the alarm
      audioPlayer.playAlarm();

      // Get and return the pressed key
      uint8_t key = keypad.getKey();

      // Update the events
      keypad.updateEvents(key);

      // Check if the enter key has been entered
      if(keypad.enterPressed())
      {
        // Switch to the interactive state and reset it
        mainState = MainState::TASK;
        task.reset();
      }

      break;
    }

    case MainState::TASK:
    {
      // Display 100 as a test
      display.displayNumber(100);
      task.update();

      break;
    }
  }
}