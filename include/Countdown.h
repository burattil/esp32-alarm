#ifndef COUNTDOWN_H
#define COUNTDOWN_H

class Countdown {
    private:
        // Variable to store when the last decrement was
        int lastDecrement;

    public:
        // Function to modify the time being set to make sure it is valid
        void modifyTime(int& time);

        // Function to reset the lastDecrement variable to start when the countdown starts
        void resetTimer();

        // Function to decrement the time in minutes and seconds
        void decrementTimeMMSS(int& time);

        // Function to decrement the time in hours and minutes
        void decrementTimeHHMM(int& time);
    };

#endif