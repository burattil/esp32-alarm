#ifndef KEYPAD_H
#define KEYPAD_H

#define NO_KEY 255

// Create a class to group the pins 
class Keypad
{
  private:
    // Row and column variables
    uint8_t rOne, rTwo, rThree, rFour;
    uint8_t cOne, cTwo, cThree, cFour; 

    // Variable to store the last key pressed
    uint8_t lastKey = NO_KEY;

    // Value to store when the last press was
    int lastPress = 0;

    // Value to determine if enter has been pressed
    bool enterEvent = false;

    // Scan once
    uint8_t scanOnce();

    // Get and return the pressed key
    uint8_t getKey();

  public: 
    // Constructor function
    Keypad(uint8_t rA, uint8_t rB, uint8_t rC, uint8_t rD, uint8_t cA, uint8_t cB, uint8_t cC, uint8_t cD);

    // Pin initializer function
    void init();

    // Update the input
    void update(int& time);

    // Function to determine if the enter button ('D') is pressed to change states
    bool enterPressed();
};

#endif