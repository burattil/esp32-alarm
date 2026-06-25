#include <Arduino.h>
#include "AudioPlayer.h"

// Constructor function
AudioPlayer::AudioPlayer(uint8_t rx, uint8_t tx)
   : DFSerial(2), // Communication set with UART2 on ESP32
   rxPin(rx), // Sets the values of private rxPin and txPin to rx and tx, respectively
   txPin(tx) {} // to be initialized in the init() function after hardware is initialized

// Initializing function
void AudioPlayer::init()
{
   // Initialize the baud rate, pins
   DFSerial.begin(9600, SERIAL_8N1, rxPin, txPin);

   // Check if there is an error, and send a message if so
   if(!player.begin(DFSerial))
   {
      // Output message and stop the program (FIX LATER)
      Serial.print("DFPlayer failed.\n\nEnsure proper connection and whether SD card is properly inserted.");
      while(true);
   }

   // Set the default value to 10
   player.volume(20);
}

// Function to play the alarm
void AudioPlayer::playAlarm()
{
   // If the alarm's duration has passed, play it again
   if(millis() - lastPlayTime >= ALARM_TIME)
   {
      // Play it, and store when it began
      player.play(1);
      lastPlayTime = millis();
   }
}