#include <Arduino.h>
#include "Task.h"

// Constructor function
Task::Task(AudioPlayer& audio, Keypad& keypad)
    // Assign the audioPlayer object to one already created
    : audioPlayer(audio),
      keypad(keypad) {} 

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

// Helper function to set the correct index
void Task::setCorrectIndex()
{
    // Loop through the Italian words to find the correct index
    for(int i = 0; i < 4; i++)
    {
        // If they match, set the correct index to that index and break the loop
        if(italianWords[i] == englishWord)
        {
            correctIndex = i;
            break;
        }
    }
}

// Helper function to create a new set of words in a randomized order
void Task::newWords()
{
    selectWords();
    shuffleWords();
    setCorrectIndex();
}

// Function to reset the task when re-entering that state
void Task::reset()
{
    // Change the state to the first state
    taskState = TaskState::WAIT_FOR_START;

    // Rerandomize the words
    newWords();
}

// Helper function to convert the selected answer to the corresponding index
uint8_t Task::convertToIndex(uint8_t answer)
{
    // Convert the answer to the corresponding index
    if(answer == 1) return 0;
    else if(answer == 2) return 1;
    else if(answer == 3) return 2;
    else if(answer == 4) return 3;

    // If the answer is invalid, return an invalid index
    return NO_KEY;
}

// Helper function to submit the answer
void Task::submitAnswer(uint8_t answerIndex)
{
    // If the answer is correct, go to the success state
    if(answerIndex == correctIndex)
    {
        taskState = TaskState::SUCCESS;
    }

    // If the ansewr is incorrect, play the alarm
    else
    {
        taskState = TaskState::FAILURE;
    }
}

// Function to determine if the user is inactive
bool Task::isInactive()
{
    // If the user is inactive, return true
    if(inactive)
    {
        // Reset the inactive variable and return true
        inactive = false;
        return true;
    }

    // Otherwise, return false
    return false;
}

// Function to determine if the task has been completed successfully
bool Task::isTaskCompleted()
{
    // If the task has been completed, return true
    if(taskCompleted)
    {
        // Reset the taskCompleted variable and return true
        taskCompleted = false;
        return true;
    }

    // Otherwise, return false
    return false;
}

// Create a function to update which state the task is in
void Task::update()
{
    // Loop through the different states
    switch(taskState)
    {
        // Have a brief wait at the beginning before entering the first state
        case(TaskState::WAIT_FOR_START):
        {
            // Check if enough time has elapsed
            if(timeElapsed(waitStartTime, WAIT_TIME))
            {
                // If so, move to the next state
                taskState = TaskState::ENGLISH_WORD;
            }

            break;
        }

        // Let the English word play
        case(TaskState::ENGLISH_WORD):
        {
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
        }

        // Wait between English and first Italian word
        case(TaskState::WAIT_ONE):
        {
            // Get and return the pressed key
            uint8_t key = keypad.getKey();

            // Update the events
            keypad.updateEvents(key);

            // Check if the user would like to hear the previous word again
            if(keypad.repeatPressed())
            {
                // Go back to the previous state
                taskState = TaskState::WAIT_FOR_START;

                // Reset the waitStartTime variable to 0 so that the wait time is reset
                waitStartTime = 0;
            }

            // Check if the user would like to hear everything again
            else if(keypad.resetPressed())
            {
                // Go back to the first state
                taskState = TaskState::WAIT_FOR_START;

                // Reset the waitStartTime variable to 0 so that the wait time is reset
                waitStartTime = 0;
            }

            // Check if enough time has elapsed
            else if(timeElapsed(waitStartTime, WAIT_TIME))
            {
                // If so, move to the next state
                taskState = TaskState::ITALIAN_WORD_ONE;
            }

            break;
        }

        // Let the first Italian word play
        case(TaskState::ITALIAN_WORD_ONE):
        {
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
        }

        // Wait between first and second Italian word
        case(TaskState::WAIT_TWO):
        {
            // Get and return the pressed key
            uint8_t key = keypad.getKey();

            // Update the events
            keypad.updateEvents(key);

            // Check if the user would like to hear the previous word again
            if(keypad.repeatPressed())
            {
                // Go back to the previous state
                taskState = TaskState::WAIT_ONE;

                // Reset the waitStartTime variable to 0 so that the wait time is reset
                waitStartTime = 0;
            }

            // Check if the user would like to hear everything again
            else if(keypad.resetPressed())
            {
                // Go back to the first state
                taskState = TaskState::WAIT_FOR_START;

                // Reset the waitStartTime variable to 0 so that the wait time is reset
                waitStartTime = 0;
            }

            // Check if enough time has elapsed
            else if(timeElapsed(waitStartTime, WAIT_TIME))
            {
                // If so, move to the next state
                taskState = TaskState::ITALIAN_WORD_TWO;
            }

            break;
        }

        // Let the second Italian word play
        case(TaskState::ITALIAN_WORD_TWO):
        {
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
        }    
        
        // Wait between second and third Italian word
        case(TaskState::WAIT_THREE):
        {
            // Get and return the pressed key
            uint8_t key = keypad.getKey();

            // Update the events
            keypad.updateEvents(key);

            // Check if the user would like to hear the previous word again
            if(keypad.repeatPressed())
            {
                // Go back to the previous state
                taskState = TaskState::WAIT_TWO;

                // Reset the waitStartTime variable to 0 so that the wait time is reset
                waitStartTime = 0;
            }

            // Check if the user would like to hear everything again
            else if(keypad.resetPressed())
            {
                // Go back to the first state
                taskState = TaskState::WAIT_FOR_START;

                // Reset the waitStartTime variable to 0 so that the wait time is reset
                waitStartTime = 0;
            }

            // Check if enough time has elapsed
            else if(timeElapsed(waitStartTime, WAIT_TIME))
            {
                // If so, move to the next state
                taskState = TaskState::ITALIAN_WORD_THREE;
            }

            break;
        }
            
        // Let the third Italian word play
        case(TaskState::ITALIAN_WORD_THREE):
        {
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
        }

        // Wait between third and fourth Italian word
        case(TaskState::WAIT_FOUR):
        {
            // Get and return the pressed key
            uint8_t key = keypad.getKey();

            // Update the events
            keypad.updateEvents(key);

            // Check if the user would like to hear the previous word again
            if(keypad.repeatPressed())
            {
                // Go back to the previous state
                taskState = TaskState::WAIT_THREE;

                // Reset the waitStartTime variable to 0 so that the wait time is reset
                waitStartTime = 0;
            }

            // Check if the user would like to hear everything again
            else if(keypad.resetPressed())
            {
                // Go back to the first state
                taskState = TaskState::WAIT_FOR_START;

                // Reset the waitStartTime variable to 0 so that the wait time is reset
                waitStartTime = 0;
            }

            // Check if enough time has elapsed
            else if(timeElapsed(waitStartTime, WAIT_TIME))
            {
                // If so, move to the next state
                taskState = TaskState::ITALIAN_WORD_FOUR;
            }

            break;
        }

        // Let the fourth Italian word play
        case(TaskState::ITALIAN_WORD_FOUR):
        {
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
                taskState = TaskState::WAIT_FOR_INPUT;
                audioStarted = false;
            }

            break;
        }

        // Wait state 
        case(TaskState::WAIT_FOR_INPUT):
        {
            // Get the pressed key from the keypad
            uint8_t key = keypad.getKey();

            // Update the events
            keypad.updateEvents(key);

            // Check if the user would like to hear the previous word again
            if(keypad.repeatPressed())
            {
                // Go back to the previous state
                taskState = TaskState::WAIT_FOUR;

                // Reset the waitStartTime variable to 0 so that the wait time is reset
                waitStartTime = 0;
            }

            // Check if the user would like to hear everything again
            else if(keypad.resetPressed())
            {
                // Go back to the first state
                taskState = TaskState::WAIT_FOR_START;

                // Reset the waitStartTime variable to 0 so that the wait time is reset
                waitStartTime = 0;
            }

            // Make sure that the input is not left open for too long
            else if(timeElapsed(waitStartTime, WAIT_FOR_INPUT_TIME))
            {
                // Set the activity 
                inactive = true;
            }

            // Convert the answer to the corresponding index
            uint8_t answerIndex = convertToIndex(key);

            // Submit the answer if it is valid
            if(answerIndex != NO_KEY)
            {
                submitAnswer(answerIndex);
            }

            break;
        }

        // Success state
        case(TaskState::SUCCESS):
        {
            // Play the success audio
            audioPlayer.playSuccess();

            // Only stay here for a second, then return to the input state
            if(timeElapsed(waitStartTime, 1000))
            {
                // Switch the taskCompleted variable to true
                taskCompleted = true;
            }

            break;
        }

        // Fail state
        case(TaskState::FAILURE):
        {
            // Play the alarm
            audioPlayer.playAlarm();
            
            // Only stay here for a second, the return to the input state
            if(timeElapsed(waitStartTime, 1000))
            {
                taskState = TaskState::WAIT_FOR_INPUT;
            }

            break;
        }
    }
}