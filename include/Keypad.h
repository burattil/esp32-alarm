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

    // Scan once
    uint8_t scanOnce();

    // Helper function to wait till it is released
    void waitForRelease();

    // Get and return the pressed key
    uint8_t getKey();

    // Return whether or not a button is being pressed
    bool isPressed();

  public: 
    // Constructor function
    Keypad(uint8_t rA, uint8_t rB, uint8_t rC, uint8_t rD, uint8_t cA, uint8_t cB, uint8_t cC, uint8_t cD);

    // Pin initializer function
    void init();

    // Get the input
    int getAndDisplayInput();
};

#endif