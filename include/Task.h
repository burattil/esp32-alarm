#ifndef TASK_H
#define TASK_H

#include "AudioPlayer.h"
#include "Keypad.h"

#define ENGLISH_FOLDER 1
#define ITALIAN_FOLDER 2
#define TOTAL_WORDS 7
#define WAIT_TIME 2000
#define WAIT_FOR_INPUT_TIME 15000

// Create a class to handle the task logic
class Task
{
    private:
        // Create a reference to the AudioPlayer object already created
        AudioPlayer& audioPlayer;

        // Create a reference to the Keypad object already created
        Keypad& keypad;

        // Create a state machine to implement logic
        enum class TaskState
        {
            WAIT_FOR_START,
            ENGLISH_WORD,
            WAIT_ONE,
            ITALIAN_WORD_ONE,
            WAIT_TWO,
            ITALIAN_WORD_TWO,
            WAIT_THREE,
            ITALIAN_WORD_THREE,
            WAIT_FOUR,
            ITALIAN_WORD_FOUR,
            WAIT_FOR_INPUT,
            SUCCESS,
            FAILURE
        };

        // Create a variable to hold the state
        TaskState taskState = TaskState::ENGLISH_WORD;

        // Create a variable to determine if the audio has started
        bool audioStarted = false;

        // Variable to determine when the wait time began
        unsigned long waitStartTime = 0;

        // Variables to store the words 
        uint16_t englishWord;
        uint16_t italianWords[4];
        uint16_t correctIndex;

        // Variable to determine if the user is inactive
        bool inactive = false;

        // Variable to determine if the user has completed the task successfully
        bool taskCompleted = false;

        // Helper function to determine if enough time has elapsed between words
        bool timeElapsed(unsigned long& startTime, unsigned long waitTime);

        // Helper function to select the words
        void selectWords();

        // Helper function to randomize the words
        void shuffleWords();

        // Helper function to set the correct index
        void setCorrectIndex();
        
        // Helper function to create a new set of words in a randomized order
        void newWords();

        // Helper function to convert the selected answer to the corresponding index
        uint8_t convertToIndex(uint8_t answer);

        // Helper function to submit the answer
        void submitAnswer(uint8_t answerIndex);

    public:
        // Constructor function
        Task(AudioPlayer& audio, Keypad& keypad);

        // State reset function
        void reset();

        // Function to determine if the user is inactive
        bool isInactive();

        // Function to determine if the task has been completed successfully
        bool isTaskCompleted();

        // Update function to continually loop through the task states
        void update();
};

#endif