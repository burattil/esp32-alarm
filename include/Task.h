#ifndef TASK_H
#define TASK_H

#include "AudioPlayer.h"

#define ENGLISH_FOLDER 1
#define ITALIAN_FOLDER 2
#define TOTAL_WORDS 7
#define WAIT_TIME 1500

// Create a class to handle the task logic
class Task
{
    private:
        // Create a reference to the AudioPlayer object already created
        AudioPlayer& audioPlayer;

        // Create a state machine to implement logic
        enum class TaskState
        {
            ENGLISH_WORD,
            WAIT_ONE,
            ITALIAN_WORD_ONE,
            WAIT_TWO,
            ITALIAN_WORD_TWO,
            WAIT_THREE,
            ITALIAN_WORD_THREE,
            WAIT_FOUR,
            ITALIAN_WORD_FOUR,
            WAIT_POST
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
        uint16_t correctChoice;

        // Helper function to determine if enough time has elapsed between words
        bool timeElapsed(unsigned long& startTime, unsigned long waitTime);

        // Helper function to select the words
        void selectWords();

        // Helper function to randomize the words
        void shuffleWords();
        
        // Helper function to create a new set of words in a randomized order
        void newWords();

    public:
        // Constructor function
        Task(AudioPlayer& audio);

        // State reset function
        void reset();

        // Update function to continually loop through the task states
        void update();
};

#endif