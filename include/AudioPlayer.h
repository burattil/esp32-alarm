#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

// Create a class to control the audio player
class AudioPlayer 
{
    private:
        // Create the DFPlayer Mini object privately
        DFRobotDFPlayerMini player;

        // Create the serial object privately
        HardwareSerial DFSerial;

        // Variables to store the Rx, Tx and Busy pins
        uint8_t rxPin, txPin, busyPin;

        // Variable to determine if the alarm is active
        int lastPlayTime = 0;

    public:
        // Constructor function
        AudioPlayer(uint8_t rx, uint8_t tx, uint8_t busy);

        // Initializer
        void init();

        // Helper function to determine if the player is busy
        bool isPlaying();

        // Play the alarm
        void playAlarm();

        // Test an English and Italian track
        void playAudio(uint8_t folder, uint16_t file);
};

#endif