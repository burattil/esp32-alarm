#include <Arduino.h>
#include "Countdown.h"

// Function to ensure that the time entered is valid
void Countdown::modifyTime(int& time)
{
    // If the time is greater than 99:59, set it to 99:59
    if(time > 9959) time = 9959;
    
    // If the seconds are greater than 59, set it to 59
    else if(time % 100 > 59) time = (time / 100) * 100 + 59;

    return;
}

// Function to reset the lastDecrement variable to start when the countdown starts
void Countdown::resetTimer()
{
    lastDecrement = millis();

    return;
}

// Function to decrement the time in minutes and seconds
void Countdown::decrementTimeMMSS(int& time)
{
    // If it has been more than a second since the last decrement, decrement the time
    if(millis() - lastDecrement > 1000) 
    {
        // Count down in terms of minutes and seconds
        if(time == 0) time = 0;  // If it is passing 0, keep it at 0
        else if(time % 100 == 0) time -= 41;  // If it is about to turn to 99, switch it to 59
        else time--; // If it is an unexceptional number, decrement it by 1

        // Save the last time it was decremented
        lastDecrement = millis();
    }

    return;
}

// Function to decrement the time in hours and minutes
void Countdown::decrementTimeHHMM(int& time)
{
    // If it has been more than a minute since the last decrement, decrement the time
    if(millis() - lastDecrement > 60000) 
    {
        // Count down in terms of hours and minutes
        if(time == 0) time = 0;  // If it is 0, keep it at 0
        else if(time % 100 == 0) time -= 41;  // If it is about to turn to 59, switch it to 59
        else time--; // If it is an unexceptional number, decrement it by 1

        // Save the last time it was decremented
        lastDecrement = millis();
    }

    return;
}