# Interactive Language-Learning Alarm

An interactive alarm created using an ESP32 microcontroller, display, keypad, a DFPlayer Mini, and a speaker. The device forces the user to complete a multiple choice style language challenge before the alarm is dismissed.

The alarm plays an English word followed by four Italian options. Selecting the correct answer silences the alarm, while incorrect answers require more attempts.

## Demonstration

https://github.com/user-attachments/assets/b564b586-7415-4338-8848-433ddb43c591

## Usage

 1. Assemble and connect the MCU to the PC.
 2. Generate the audio files using the Python script and Words.csv in `tools/audio_generator`.
 3. Copy the generated audio files in `tools/audio_generator/audio` to the microSD card, labelling the `english` and `italian` folders as `01` and `02`, respectively. Copy the provided folder `03` to the microSD card.
 4. Upload the firmware to the MCU.
 5. Enter the desired alarm duration using the keypad.
 6. Press the Enter key to begin the countdown.
 7. When the alarm sounds, click the Enter key again to proceed to the vocabulary challenge.
 8. Complete the vocabulary challenge to silence the alarms by clicking 1, 2, 3, or 4.
 9. Repeat the steps 5–8 as many times as needed.

## Features

 - Countdown timer
 - 4-digit TM1637 display
 - 4x4 keypad user interface
 - Audio playback using the DFPlayer Mini and speaker
 - Randomized vocabulary challenges
 - Automatic audio generation using Python
 - Non-blocking state-machine architecture

## Hardware

 - ESP32
 - TM1637 Display
 - 4x4 Matrix Keypad
 - DFPlayer Mini MP3 Module
 - 3 W 8 Ω Speaker
 - Power Supply

## Software

### Embedded Firmware

 - C++
 - PlatformIO
 - Manages keypad input and countdown
 - Displays updates
 - Controls audio playback
 - Organizes vocabulary challenge logic
 - Controls state transitions

### Audio Generation Software

A Python script is included to automatically generate audio files from a vocabulary list called `Words.csv` (usable example provided in `tools/audio_generator`).

The script reads from Words.csv in the following format:

ID,English,Italian
1,hello,ciao

Using Google Text-to-Speech (gTTS), the English and Italian words are generated and stored in separate folders labelled `english` and `italian`, respectively.

## Configuration

Any of the pins used on the ESP32 for the 4x4 keypad, TM1637 Display, and DFPlayer Mini can be modified in `main.cpp`.

The volume of the speaker can be adjusted in `AudioPlayer.h`.

Several parameters in `Task.h` can be modified: 

    - TOTAL_WORDS
        - The total number of vocabulary words ranging anywhere from 4–999 (MUST match the number in Words.csv).

    - WAIT_TIME
        - The wait time between the audio prompts.

    - WAIT_FOR_INPUT_TIME
        - Maximum time allowed for answering before the alarm resumes.

## Wiring

| Component | ESP32 Pins |
|------------|------------|
| TM1637 VCC | 3.3 V |
| TM1637 CLK | GPIO 5 |
| TM1637 DIO | GPIO 18 |
| Keypad Row 1 | GPIO 32 |
| Keypad Row 2 | GPIO 33 |
| Keypad Row 3 | GPIO 25 |
| Keypad Row 4 | GPIO 26 |
| Keypad Column 1 | GPIO 27 |
| Keypad Column 2 | GPIO 14 |
| Keypad Column 3 | GPIO 12 |
| Keypad Column 4 | GPIO 13 |
| DFPlayer Mini VIN | 5 V |
| DFPlayer Mini TX | GPIO 16 |
| DFPlayer Mini RX | GPIO 17 |
| DFPlayer Mini BUSY | GPIO 23
| Speaker | DFPlayer Mini SPK1/SPK2 |

<br>

<p>
 <img width="826" height="298" alt="image" src="https://github.com/user-attachments/assets/44eba96a-53af-4ecc-8259-5e600b8f9f1f">
</p>

## Keypad Controls

### Timer Setup

| Key | Function |
|-----|----------|
| 0–9 | Enter timer value | 
| A | Delete |
| B | Clear |
| D | Enter |

### Countdown

| Key | Function |
|-----|----------|
| B | Reset |
| * | Pause |
| # | Resume |

### Vocabulary Challenge

| Key | Function |
|-----|----------|
| 1—4 | Answer selections |
| A | Repeat last word |
| B | Restart words |

## State Machine

The embedded code is implemented as a non-blocking state machine. 

The states include: 

 - SETTING_TIME
 - COUNTING_DOWN
 - PAUSED
 - ALARM
 - TASK

The TASK state has several substates

 - WAIT_FOR_START
 - ENGLISH_WORD
 - WAIT_ONE
 - ITALIAN_WORD_ONE
 - WAIT_TWO
 - ITALIAN_WORD_TWO
 - WAIT_THREE
 - ITALIAN_WORD_THREE
 - WAIT_FOUR
 - ITALIAN_WORD_FOUR
 - WAIT_FOR_INPUT
 - SUCCESS
 - FAILURE

 This design allows the system to be completely responsive while managing keypad input, display updates, timing, and audio playback simultaneously.

 INCLUDE STATE MACHINE DIAGRAM HERE.

## Author

Luca Burattini
