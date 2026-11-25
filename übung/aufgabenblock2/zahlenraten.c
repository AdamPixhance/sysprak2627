/*
Aufgabe 2.5

In diesem Beispiel soll das ” Spiel“ Zahlenraten implementiert werden.

1. Der Computer erzeugt eine Zufallszahl zwischen 1 und 100, die es zu erraten gilt. Die Spielerin wird solange aufgefordert eine Zahl einzugeben (zu raten), bis die Zahl erraten wurde. Nach jeder Eingabe gibt der Computer an, ob die Zufallszahl kleiner, gr¨ oßer oder gleich der geratenen Zahl ist. Das Spiel ist beendet, sobald die Zahl erraten wurde. Die Anzahl der Rateversuche wird mitgez¨ ahlt und am Spielende ausgegeben. Nach jedem Spiel wird die Spielerin gefragt, ob noch eine Runde gespielt werden soll. Wird diese Abfrage verneint, wird das Programm beendet.

2. Das Zahlenraten soll nun um eine Bestenliste (der besten 10), die in einer Datei abgespeichert ist, erweitert werden.
Die Eintr¨ age in der Bestenliste sind nach der Anzahl der Rateversuche geordnet. Nummer 1 in der Bestenliste hat die wenigsten Versuche gebraucht, um eine Zahl korrekt zu raten. Bei Gleichstand wird der alte Eintrag nach unten verschoben (der neue Eintrag vor dem ¨ alteren eingefügt). Um einen ¨ Uberblick ¨ uber die abgeschlossenen Spiele zu haben, wird in eine Logdatei die jeweils zu erratende Zahl und die Anzahl der ben¨ otigten Rateversuche protokolliert.
*/

// includes
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

// gameRootCycle constants
#define GAME_ROOT_CYCLE_QUIT 0
#define GAME_ROOT_CYCLE_CRASH -1
#define GAME_ROOT_CYCLE_CONTINUE 1

// oneGameCycle constants
#define STARTING_MIN_NUMBER 0
#define STARTING_MAX_NUMBER 1000
#define oneGameCycle_IS_GUESSED_CORRECT 1
#define oneGameCycle_IS_GUESSED_WRONG 0

// innerGameCycleGuessLoop constant
#define INNER_GAME_CYCLE_GUESS_LOOP_CORRECT 1
#define INNER_GAME_CYCLE_GUESS_LOOP_WRONG 0

// gameContinueCheck constants
const char *CONTINUE_KEYWORD = "yes";
#define CONTINUE_STRING_CHECK_SIZE 32

// oneGuessInstance constants
#define ONE_GUESS_INSTANCE_WRONG 0
#define ONE_GUESS_INSTANCE_RIGHT 1
#define ONE_GUESS_INSTANCE_ERROR -99
#define USER_GUESS_NUMBER_AS_STRING_SIZE 32

// continuecheckUserInput
#define CONTINUE_CHECK_USER_INPUT_FAILED -1
#define CONTINUE_CHECK_USER_INPUT_YES 1
#define CONTINUE_CHECK_USER_INPUT_NO 0

// takeInputFromUser constants
#define takeInputfromUserFailed -1
#define TAKE_INPUT_FROM_USER_SUCCESS 1

// converStringToNumber constant
#define SUCCESS_convertStringToNumber 1
#define ERROR_convertStringToNumber -1

// help functions forward declaration
int gameRootCycle();
int oneGameCycle();
void gameLooper();
int gameContinueCheck();
int continueCheckUserInput();
int takeInputFromUser(char *whereToWriteString, size_t InputStringSize);
int generateNewRandomNumber();
int innerGameCycleGuessLoop(int currentRandomNumber);
int oneGuessInstance(int *currentMin, int *currentMax, int currentRandomNumber);
int compareGuessedNumbers(int *currentMin, int *currentMax, int userGuessedNumber, int currentRandomNumber);

// -----------------------
// start of main
int main(int argc, char *argv[])
{
    puts("Programm started");
    int gameQuitStatus = gameRootCycle();
    if (gameQuitStatus == GAME_ROOT_CYCLE_CRASH)
    {
        puts("The game crashed. Quitting the programm now...");
        return 1;
    }
    else if (gameQuitStatus == GAME_ROOT_CYCLE_QUIT)
    {
        puts("Quitting game. Thanks for playing today!");
    }

    return 0;
}
// end of main
// -----------------------

// help functions definitions
// -----------------------
int gameRootCycle()
{
    srand(time(0)); // seed the random number generator

    gameLooper();
    return GAME_ROOT_CYCLE_QUIT; // normal gameQuitStatus
}

// -----------------------
int oneGameCycle()
{
    int currentRandomNumber = generateNewRandomNumber();
    puts("Random Number has been Generated. The game has started!\n\n");
    int isGuessedCorrect = oneGameCycle_IS_GUESSED_WRONG;
    while (isGuessedCorrect != oneGameCycle_IS_GUESSED_CORRECT)
    {
        int innerGameCycleGuessLoopStatus = innerGameCycleGuessLoop(currentRandomNumber);
        if (innerGameCycleGuessLoopStatus == INNER_GAME_CYCLE_GUESS_LOOP_CORRECT)
        {
            isGuessedCorrect = oneGameCycle_IS_GUESSED_CORRECT;
        }
    }
    return 0; // on correct guess
}

// -----------------------
void gameLooper()
{
    int gameContinue = GAME_ROOT_CYCLE_CONTINUE;
    while (gameContinue == GAME_ROOT_CYCLE_CONTINUE)
    {
        oneGameCycle();
        gameContinue = gameContinueCheck();
    }
}

// -----------------------
int gameContinueCheck()
{
    puts("Do you want to continue playing with a new number?");
    printf("If you want to play again, type %s, otherwise type no or press Enter with no input:\n", CONTINUE_KEYWORD);
    int continueCheckUserInputAnswer = continueCheckUserInput();
    if (continueCheckUserInputAnswer != CONTINUE_CHECK_USER_INPUT_YES)
    {
        puts("You chose not to play again");
        return GAME_ROOT_CYCLE_QUIT;
    }
    puts("Good luck with a new random number! how many tries do you need this time?\n");
    return GAME_ROOT_CYCLE_CONTINUE;
}

// -----------------------
int continueCheckUserInput()
{
    char bufferInputFromUser[CONTINUE_STRING_CHECK_SIZE];
    int takeInputromUserStatus = takeInputFromUser(bufferInputFromUser, sizeof(bufferInputFromUser));
    if (takeInputromUserStatus == takeInputfromUserFailed)
    {
        puts("continueCheckUserInput failed");
        return CONTINUE_CHECK_USER_INPUT_FAILED;
    }
    if (strcmp(bufferInputFromUser, CONTINUE_KEYWORD) == 0)
    {
        return CONTINUE_CHECK_USER_INPUT_YES;
    }
    return CONTINUE_CHECK_USER_INPUT_NO;
}

// -----------------------
int takeInputFromUser(char *whereToWriteString, size_t InputStringSize)
{
    char bufferInputFromUser[InputStringSize];
    // taking user input
    // checking for input error
    if ((fgets(bufferInputFromUser, sizeof(bufferInputFromUser), stdin)) == NULL)
    {
        puts("takeInputFromUser failed");
        return takeInputfromUserFailed;
    }

    // checking for too much input
    size_t newlineIndex = strcspn(bufferInputFromUser, "\n");
    if (bufferInputFromUser[newlineIndex] != '\n')
    {
        int bufferChar = getchar();
        if (bufferChar != '\n' && bufferChar != EOF) // double checks to make sure the input wasnt exactly the correct size with a hanging \n
        {
            puts("takeInputFromUser failed");
            while (bufferChar != EOF && bufferChar != '\n') // cleans up any extra input
            {
                bufferChar = getchar();
            }
            return takeInputfromUserFailed;
        }
    }

    // removing the trailing newline if it exists
    bufferInputFromUser[newlineIndex] = '\0';

    // copy the new input into the target string
    strcpy(whereToWriteString, bufferInputFromUser);
    return TAKE_INPUT_FROM_USER_SUCCESS;
}

// -----------------------
int convertStringToNumber(char *stringToConvert, int *whereToSaveNumber)
{
    char *endptr;

    *whereToSaveNumber = strtol(stringToConvert, &endptr, 10);

    if (endptr == stringToConvert)
    {
        printf("No digits were found.\n");
        return ERROR_convertStringToNumber;
    }
    else if (*endptr != '\0')
    {
        printf("Invalid character: %c\n", *endptr);
        return ERROR_convertStringToNumber;
    }

    return SUCCESS_convertStringToNumber;
}

// -----------------------
int generateNewRandomNumber()
{
    return rand() % (STARTING_MAX_NUMBER - STARTING_MIN_NUMBER + 1) + STARTING_MIN_NUMBER;
}

// -----------------------
int innerGameCycleGuessLoop(int currentRandomNumber)
{
    int currentMin = STARTING_MIN_NUMBER;
    int currentMax = STARTING_MAX_NUMBER;

    int guessCounter = 0;

    int oneGuessInstanceStatus = ONE_GUESS_INSTANCE_WRONG;
    while (oneGuessInstanceStatus != ONE_GUESS_INSTANCE_RIGHT)
    {
        guessCounter += 1;
        printf("\nThis is your #%d guess:\n", guessCounter);
        oneGuessInstanceStatus = oneGuessInstance(&currentMin, &currentMax, currentRandomNumber);
        if (oneGuessInstanceStatus == ONE_GUESS_INSTANCE_ERROR)
        {
            guessCounter -= 1;
        }
        else if (oneGuessInstanceStatus == ONE_GUESS_INSTANCE_RIGHT)
        {
            printf("You have guessed %d times.\n", guessCounter);
        }
        else if (oneGuessInstanceStatus != oneGameCycle_IS_GUESSED_WRONG)
        {
            puts("Something went really wrong with the programm. Please click ctrl + c and try again.");
        }
    }
    return INNER_GAME_CYCLE_GUESS_LOOP_CORRECT;
}

// -----------------------
int oneGuessInstance(int *currentMin, int *currentMax, int currentRandomNumber)
{
    int oneGuessInstanceResponse;
    printf("Guess the number between %d and %d.\n", *currentMin, *currentMax);

    int userGuessedNumber;
    char userGuessedNumberAsString[USER_GUESS_NUMBER_AS_STRING_SIZE];
    int takeInputFromUserResponse = takeInputFromUser(userGuessedNumberAsString, sizeof(userGuessedNumberAsString));

    if (takeInputFromUserResponse != TAKE_INPUT_FROM_USER_SUCCESS)
    {
        puts("oneGuessInstanceError");
        oneGuessInstanceResponse = ONE_GUESS_INSTANCE_ERROR;
    }
    else
    {
        int converStringToNumberStatus = convertStringToNumber(userGuessedNumberAsString, &userGuessedNumber);
        if (converStringToNumberStatus == ERROR_convertStringToNumber)
        {
            puts("error handling the user input, please try again");
            oneGuessInstanceResponse = ONE_GUESS_INSTANCE_ERROR;
        }
        else
        {
            oneGuessInstanceResponse = compareGuessedNumbers(currentMin, currentMax, userGuessedNumber, currentRandomNumber);
        }
    }

    return oneGuessInstanceResponse;
}

int compareGuessedNumbers(int *currentMin, int *currentMax, int userGuessedNumber, int currentRandomNumber)
{
    int compareGuessedNumbersResponse = ONE_GUESS_INSTANCE_WRONG;

    if (userGuessedNumber < *currentMin || userGuessedNumber > *currentMax)
    {
        puts("Please guess within the given range!");
        compareGuessedNumbersResponse = ONE_GUESS_INSTANCE_ERROR;
    }
    else if (userGuessedNumber > currentRandomNumber)
    {
        puts("Your guess was too high");
        *currentMax = userGuessedNumber - 1;
        compareGuessedNumbersResponse = ONE_GUESS_INSTANCE_WRONG;
    }
    else if (userGuessedNumber < currentRandomNumber)
    {
        puts("Your guess was too low");
        *currentMin = userGuessedNumber + 1;
        compareGuessedNumbersResponse = ONE_GUESS_INSTANCE_WRONG;
    }
    else if (userGuessedNumber == currentRandomNumber) // the correct answer was given
    {
        puts("You guessed right!");
        compareGuessedNumbersResponse = ONE_GUESS_INSTANCE_RIGHT;
    }

    return compareGuessedNumbersResponse;
}