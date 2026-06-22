#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <DFRobotDFPlayerMini.h>

// Create a class to control the audio player
class AudioPlayer 
{
    private:
        // Create the DFPlayer Mini object privately
        DFRobotDFPlayerMini player;

        // Create the serial object privately
        HardwareSerial DFSerial;

        // Variables to store the Rx and Tx pins
        uint8_t rxPin, txPin;

    public:
        // Constructor function
        AudioPlayer(uint8_t rx, uint8_t tx);

        // Initializer
        void init();

        void playAlarm();
};

#endif