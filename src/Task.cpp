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

// Function to reset the state of the task
void Task::reset()
{
    // Change the state to the first state
    taskState = TaskState::ENGLISH_WORD;
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
                audioPlayer.playAudio(ENGLISH_FOLDER, 1);
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

        case(TaskState::ITALIAN_WORD_ONE):
            // Only play if it is not already playing
            if(!audioStarted)
            {
                // Play the first required Italian audio and make audioStarted true
                audioPlayer.playAudio(ITALIAN_FOLDER, 1);
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

        // Wait between English and first Italian word
        case(TaskState::WAIT_TWO):
            // Check if enough time has elapsed
            if(timeElapsed(waitStartTime, WAIT_TIME))
            {
                // If so, move to the next state
                taskState = TaskState::ITALIAN_WORD_TWO;
            }

            break;

        case(TaskState::ITALIAN_WORD_TWO):
            audioPlayer.playAlarm();

            break;
            
    }
}