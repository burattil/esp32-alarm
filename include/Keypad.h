#ifndef KEYPAD_H
#define KEYPAD_H

#define DEBOUNCE_TIME 20
#define RESET_KEY 250
#define NO_KEY 251
#define DELETE_KEY 252
#define ENTER_KEY 253
#define PAUSE_KEY 254
#define RESUME_KEY 255

// Create a class to group the pins 
class Keypad
{
  private:
    // Row and column variables
    uint8_t rOne, rTwo, rThree, rFour,
            cOne, cTwo, cThree, cFour; 

    // Variable to store the last key pressed
    uint8_t lastKey = NO_KEY;

    // Value to store when the last press was
    int lastPress = 0;

    // Values to determine if enter, pause, or resume has been pressed
    bool enterEvent = false;
    bool pauseEvent = false;
    bool resumeEvent = false;
    bool resetEvent = false;

    // Scan once
    uint8_t scanOnce();

  public: 
    // Constructor function
    Keypad(uint8_t rA, uint8_t rB, uint8_t rC, uint8_t rD, uint8_t cA, uint8_t cB, uint8_t cC, uint8_t cD);

    // Pin initializer function
    void init();

    // Get and return the pressed key
    uint8_t getKey();

    // Update the input
    void updateTime(uint8_t key, int& time);

    // Update the events
    void updateEvents(uint8_t key);

    // Function to determine if ENTER is pressed to change states
    bool enterPressed();

    // Function to determine if PAUSE is pressed to change states
    bool pausePressed();

    // Function to determine if RESUME is pressed to change states
    bool resumePressed();

    // Function to determine if RESET is pressed to change states
    bool resetPressed();
};

#endif