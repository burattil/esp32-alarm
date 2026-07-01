#include <Arduino.h>
#include "Task.h"

// Constructor function
Task::Task(AudioPlayer& audio)
    // Assign the audioPlayer object to one already created
    : audioPlayer(audio) {} 

// Helper function to determine if enough time has elapsed
bool Task::timeElapsed(unsigned long& startTime, unsigned long waitTime)
{
    // Check if the startTime is "now" (left as 0)
    if(startTime == 0)
    {
        // Set it to the "actual" beginning
        startTime = millis();
    }

    // Otherwise, check if enough time has elapsed
    if(millis() - startTime >= waitTime)
    {
        // Reset the startTime and return true
        startTime = 0;
        return true;
    }

    // If enough time hasn't elapsed, keep returning false
    return false;
}

// Helper function to select the words
void Task::selectWords()
{
    // Randomly select the English word
    englishWord = random(1, TOTAL_WORDS + 1);

    // Make sure the corresponding Italian word is selected already
    italianWords[0] = englishWord;

    // Randomly select the other three Italian words
    for(int i = 1; i < 4; i++)
    {
        // Create a "unique" variable to determine if the word is unique
        bool unique = false;

        // If it is not unique, keep generating a new word until it is
        while(!unique)
        {
            // Generate the next word
            italianWords[i] = random(1, TOTAL_WORDS + 1);

            // Update the unique variable to true, then check if it is actually unique
            unique = true;

            // Check if the new word is unique
            for(int k = 0; k < i; k++)
            {
                if(italianWords[i] == italianWords[k])
                {
                    unique = false;
                    break;
                }
            }
        }
    }
}

// Helper function to randomize the words
void Task::shuffleWords()
{
    // Fisher-Yates shuffle algorithm to randomize the order of the Italian words
    for(int i = 3; i > 0; i--)
    {
        // Randomly select a number between 0 and i
        int j = random(0, i + 1);

        // Swap the two words
        uint16_t temp = italianWords[i];
        italianWords[i] = italianWords[j];
        italianWords[j] = temp;
    }
}

// Helper function to create a new set of words in a randomized order
void Task::newWords()
{
    // Select the words
    selectWords();

    // Shuffle the words
    shuffleWords();
}

// Function to reset the task when re-entering that state
void Task::reset()
{
    // Change the state to the first state
    taskState = TaskState::ENGLISH_WORD;

    // Rerandomize the words
    newWords();
}

// Create a function to update which state the task is in
void Task::update()
{
    // Loop through the different states
    switch(taskState)
    {
        // Let the English word play
        case(TaskState::ENGLISH_WORD):
            // Only play if it is not already playing
            if(!audioStarted)
            {
                // Play the required English audio and make audioStarted true
                audioPlayer.playAudio(ENGLISH_FOLDER, englishWord);
                audioStarted = true;
            }

            // If audioStarted is true but it is no longer playing
            else if(audioStarted && !audioPlayer.isPlaying())
            {
                // Go to the next state and make audioStarted false again
                taskState = TaskState::WAIT_ONE;
                audioStarted = false;
            }

            break;

        // Wait between English and first Italian word
        case(TaskState::WAIT_ONE):
            // Check if enough time has elapsed
            if(timeElapsed(waitStartTime, WAIT_TIME))
            {
                // If so, move to the next state
                taskState = TaskState::ITALIAN_WORD_ONE;
            }

            break;

        // Let the first Italian word play
        case(TaskState::ITALIAN_WORD_ONE):
            // Only play if it is not already playing
            if(!audioStarted)
            {
                // Play the first required Italian audio and make audioStarted true
                audioPlayer.playAudio(ITALIAN_FOLDER, italianWords[0]);
                audioStarted = true;
            }

            // If audioStarted is true but it is no longer playing
            else if(audioStarted && !audioPlayer.isPlaying())
            {
                // Go to the next state and make audioStarted false again
                taskState = TaskState::WAIT_TWO;
                audioStarted = false;
            }

            break;

        // Wait between first and second Italian word
        case(TaskState::WAIT_TWO):
            // Check if enough time has elapsed
            if(timeElapsed(waitStartTime, WAIT_TIME))
            {
                // If so, move to the next state
                taskState = TaskState::ITALIAN_WORD_TWO;
            }

            break;

        // Let the second Italian word play
        case(TaskState::ITALIAN_WORD_TWO):
            // Only play if it is not already playing
            if(!audioStarted)
            {
                // Play the second required Italian audio and make audioStarted true
                audioPlayer.playAudio(ITALIAN_FOLDER, italianWords[1]);
                audioStarted = true;
            }

            // If audioStarted is true but it is no longer playing
            else if(audioStarted && !audioPlayer.isPlaying())
            {
                // Go to the next state and make audioStarted false again
                taskState = TaskState::WAIT_THREE;
                audioStarted = false;
            }

            break;
            
        // Wait between second and third Italian word
        case(TaskState::WAIT_THREE):
            // Check if enough time has elapsed
            if(timeElapsed(waitStartTime, WAIT_TIME))
            {
                // If so, move to the next state
                taskState = TaskState::ITALIAN_WORD_THREE;
            }

            break;
            
        // Let the third Italian word play
        case(TaskState::ITALIAN_WORD_THREE):
            // Only play if it is not already playing
            if(!audioStarted)
            {
                // Play the third required Italian audio and make audioStarted true
                audioPlayer.playAudio(ITALIAN_FOLDER, italianWords[2]);
                audioStarted = true;
            }

            // If audioStarted is true but it is no longer playing
            else if(audioStarted && !audioPlayer.isPlaying())
            {
                // Go to the next state and make audioStarted false again
                taskState = TaskState::WAIT_FOUR;
                audioStarted = false;
            }

            break;

        // Wait between third and fourth Italian word
        case(TaskState::WAIT_FOUR):
            // Check if enough time has elapsed
            if(timeElapsed(waitStartTime, WAIT_TIME))
            {
                // If so, move to the next state
                taskState = TaskState::ITALIAN_WORD_FOUR;
            }

            break;

        // Let the fourth Italian word play
        case(TaskState::ITALIAN_WORD_FOUR):
            // Only play if it is not already playing
            if(!audioStarted)
            {
                // Play the fourth required Italian audio and make audioStarted true
                audioPlayer.playAudio(ITALIAN_FOLDER, italianWords[3]);
                audioStarted = true;
            }

            // If audioStarted is true but it is no longer playing
            else if(audioStarted && !audioPlayer.isPlaying())
            {
                // Go to the next state and make audioStarted false again
                taskState = TaskState::WAIT_POST;
                audioStarted = false;
            }

            break;

        // Wait state 
        case(TaskState::WAIT_POST):
            // Check if enough time has elapsed
            if(timeElapsed(waitStartTime, WAIT_TIME))
            {
                // If so, move to the next state
                taskState = TaskState::ENGLISH_WORD;
            }

            break;
    }
}