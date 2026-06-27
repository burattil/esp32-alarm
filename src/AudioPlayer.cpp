#include <Arduino.h>
#include "AudioPlayer.h"

// Constructor function
AudioPlayer::AudioPlayer(uint8_t rx, uint8_t tx, uint8_t busy)
   : DFSerial(2), // Communication set with UART2 on ESP32
   rxPin(rx), // Sets the values of private rxPin and txPin to rx and tx, respectively
   txPin(tx), // to be initialized in the init() function after hardware is initialized
   busyPin(busy) {} // Sets the location of the busy pin on the ESP32

// Initializing function
void AudioPlayer::init()
{
   // Initialize the baud rate, pins
   DFSerial.begin(9600, SERIAL_8N1, rxPin, txPin);

   // Initialize the busy pin
   pinMode(busyPin, INPUT);

   // Check if there is an error, and send a message if so
   if(!player.begin(DFSerial))
   {
      // Output message and stop the program (FIX LATER)
      Serial.print("DFPlayer failed.\n\nEnsure proper connection and whether SD card is properly inserted.");
      while(true);
   }

   // Set the default value to 20
   player.volume(20);
}

// Helper function to determine if the player is busy
bool AudioPlayer::isPlaying()
{
   return digitalRead(busyPin) == LOW; 
}

// Function to play the alarm 
void AudioPlayer::playAlarm()
{
   // Only send the command if it is not currently playing
   if(!isPlaying())
   {
      player.playFolder(3, 1);
   }
}

// Function to play the desired word
void AudioPlayer::playAudio(uint8_t folder, uint16_t file)
{
   player.playFolder(folder, file);
}