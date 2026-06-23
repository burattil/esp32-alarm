#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

#define ALARM_TIME 8500 // Length of alarm mp3 file

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

        // Variable to determine if the alarm is active
        int lastPlayTime = 0;

    public:
        // Constructor function
        AudioPlayer(uint8_t rx, uint8_t tx);

        // Initializer
        void init();

        void playAlarm();
};

#endif