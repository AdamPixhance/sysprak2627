/*
Aufgabe 2.5

In diesem Beispiel soll das ” Spiel“ Zahlenraten implementiert werden.

1. Der Computer erzeugt eine Zufallszahl zwischen 1 und 100, die es zu erraten gilt. Die Spielerin wird solange aufgefordert eine Zahl einzugeben (zu raten), bis die Zahl erraten wurde. Nach jeder Eingabe gibt der Computer an, ob die Zufallszahl kleiner, gr¨ oßer oder gleich der geratenen Zahl ist. Das Spiel ist beendet, sobald die Zahl erraten wurde. Die Anzahl der Rateversuche wird mitgez¨ ahlt und am Spielende ausgegeben. Nach jedem Spiel wird die Spielerin gefragt, ob noch eine Runde gespielt werden soll. Wird diese Abfrage verneint, wird das Programm beendet.

2. DasZahlenraten soll nun um eine Bestenliste (der besten 10), die in einer Datei abgespeichert ist, erweitert werden.
Die Eintr¨ age in der Bestenliste sind nach der Anzahl der Rateversuche geordnet. Nummer 1 in der Bestenliste hat die wenigsten Versuche gebraucht, um eine Zahl korrekt zu raten. Bei Gleichstand wird der alte Eintrag nach unten verschoben (der neue Eintrag vor dem ¨ alteren eingef¨ ugt). Um einen ¨ Uberblick ¨ uber die abgeschlossenen Spiele zu haben, wird in eine Logdatei die jeweils zu erratende Zahl und die Anzahl der ben¨ otigten Rateversuche protokolliert.
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
#define STARTING_MAX_NUMBER 100
#define IS_GUESSED_WRONG 0
#define IS_GUESSED_CORRECT 1

// innerGameCycleGuessLoop constant
#define INNER_GAME_CYCLE_GUESS_LOOP_CORRECT 1
#define INNER_GAME_CYCLE_GUESS_LOOP_WRONG 0

// gameContinueCheck constants
const char *CONTINUE_KEYWORD = "yes";
#define CONTINUE_STRING_CHECK_SIZE 32

// oneGuessInstance constants
#define ONE_GUESS_INSTANCE_WRONG 0
#define ONE_GUESS_INSTANCE_RIGHT 1

// continuecheckUserInput
#define CONTINUE_CHECK_USER_INPUT_FAILED -1
#define CONTINUE_CHECK_USER_INPUT_YES 1
#define CONTINUE_CHECK_USER_INPUT_NO 0

// takeInputFromUser constants
#define takeInputfromUserFailed -1
#define takeInputfromUserSuccess 1

// help functions forward declaration
int gameRootCycle();
int oneGameCycle();
void gameLooper();
int gameContinueCheck();
int continueCheckUserInput();
int takeInputFromUser(char *whereToWriteString, size_t InputStringSize);
int generateNewRandomNumber();
int innerGameCycleGuessLoop();
int oneGuessInstance(int *currentMin, int *currenMax);

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
    // TODO: The actual game cycle
    int currentRandomNumber = generateNewRandomNumber();
    int isGuessedCorrect = IS_GUESSED_WRONG;
    while (isGuessedCorrect != IS_GUESSED_CORRECT)
    {
        isGuessedCorrect = innerGameCycleGuessLoop();
    }
    return 0;
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
    int continueCheckUserInputAnswer = continueCheckUserInput();
    if (continueCheckUserInputAnswer != CONTINUE_CHECK_USER_INPUT_YES)
    {
        return GAME_ROOT_CYCLE_QUIT;
    }
    return GAME_ROOT_CYCLE_CONTINUE;
}

// -----------------------
int continueCheckUserInput()
{
    size_t InputStringSize = CONTINUE_STRING_CHECK_SIZE;
    char *bufferInputFromUser;
    int takeInputromUserStatus = takeInputFromUser(bufferInputFromUser, InputStringSize);
    if (takeInputromUserStatus == takeInputfromUserFailed)
    {
        puts("continueCheckUserInput failed");
        return CONTINUE_CHECK_USER_INPUT_FAILED;
    }
    if (strcmp(bufferInputFromUser, CONTINUE_KEYWORD))
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
    return takeInputfromUserSuccess;
}

// -----------------------
int convertStringToNumber(char *stringToConvert)
{
    char *endptr;
    long int num;

    num = strtol(stringToConvert, &endptr, 10);

    if (endptr == stringToConvert)
    {
        printf("No digits were found.\n");
    }
    else if (*endptr != '\0')
    {
        printf("Invalid character: %c\n", *endptr);
    }

    return num;
}

// -----------------------
int generateNewRandomNumber()
{
    return rand() % (STARTING_MAX_NUMBER - STARTING_MIN_NUMBER + 1) + STARTING_MIN_NUMBER;
}

// -----------------------
int innerGameCycleGuessLoop()
{
    int currentMin = STARTING_MIN_NUMBER;
    int currentMax = STARTING_MAX_NUMBER;
    int guessCounter = 0;
    int oneGuessInstanceStatus = ONE_GUESS_INSTANCE_WRONG;
    while (oneGuessInstanceStatus != ONE_GUESS_INSTANCE_RIGHT)
    {
        guessCounter += 1;
        oneGuessInstance(&currentMin, &currentMax);
    }
    return INNER_GAME_CYCLE_GUESS_LOOP_CORRECT;
}

// -----------------------
int oneGuessInstance(int *currentMin, int *currenMax)
{
    int userCurrentGuess; // TODO:
}