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
#define TAKE_INPUT_FROM_USER_FAILED -1
#define TAKE_INPUT_FROM_USER_SUCCESS 1

// converStringToNumber constant
#define SUCCESS_convertStringToNumber 1
#define ERROR_convertStringToNumber -1

// log file constants
#define LOGFILE_NAME "numberguesser_log.txt"
#define TOP_TEN_NAME "numberguesser_highscores.txt"
#define USERNAME_ARRAY_SIZE 32

// askForUsername constants
#define askForUsername_FAILED -1
#define askForUsername_SUCCESS 1

// prepareLogInfo constats
#define prepareLogInfo_READY 1
#define prepareLogInfo_ERROR -1

// checkIfFileExists_TRUE constants
#define checkIfFileExists_TRUE 0
#define checkIfFileExists_FALSE 1

// createNewFile constants
#define createNewFile_SUCCESS 0
#define createNewFile_FAIL 1

// WriteNewLogEntry constants
#define WriteNewLogEntry_SUCCESS 0
#define WriteNewLogEntry_FAIL 1

// WriteNewTopTenEntryStatus constants
#define WriteNewTopTenEntryStatus_SUCCESS 0
#define WriteNewTopTenEntryStatus_FAIL 1

// structs
struct logEntry
{
    int theNumberToGuess;
    int neededNumOfGuesses;
    time_t timeOfAttemptStart;
    char userName[USERNAME_ARRAY_SIZE];
};

// help functions forward declaration
int gameRootCycle(struct logEntry logEntryMain);
int oneGameCycle(struct logEntry logEntryMain);
void gameLooper(struct logEntry logEntryMain);
int gameContinueCheck();
int continueCheckUserInput();
int takeInputFromUser(char *whereToWriteString, size_t InputStringSize);
int generateNewRandomNumber();
int innerGameCycleGuessLoop(int currentRandomNumber, struct logEntry logEntryMain);
int oneGuessInstance(int *currentMin, int *currentMax, int currentRandomNumber);
int compareGuessedNumbers(int *currentMin, int *currentMax, int userGuessedNumber, int currentRandomNumber);
int prepareLogInfo(struct logEntry *logEntryMain);
int askForUsername(char *userName);
int WriteNewLogEntry(struct logEntry logEntryMain);
int checkIfFileExists(char *fileNameToCheckFor);
int createNewFile(char *newFileName);
int WriteNewTopTenEntry(struct logEntry logEntryMain);

int main(void)
{
    puts("Programm started");

    struct logEntry logEntryMain;
    int prepareLogInfoStatus = prepareLogInfo(&logEntryMain);
    if (prepareLogInfoStatus != prepareLogInfo_READY)
    {
        puts("Error during prepareLogInfo. Exiting programm now");
        return 1;
    }

    int gameQuitStatus = gameRootCycle(logEntryMain);
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

// help functions definitions
int gameRootCycle(struct logEntry logEntryMain)
{
    srand(time(0)); // seed the random number generator

    gameLooper(logEntryMain);
    return GAME_ROOT_CYCLE_QUIT; // normal gameQuitStatus
}

int oneGameCycle(struct logEntry logEntryMain)
{
    int currentRandomNumber = generateNewRandomNumber();
    logEntryMain.theNumberToGuess = currentRandomNumber;
    puts("Random Number has been Generated. The game has started!\n\n");

    int isGuessedCorrect = oneGameCycle_IS_GUESSED_WRONG;
    while (isGuessedCorrect != oneGameCycle_IS_GUESSED_CORRECT)
    {
        int innerGameCycleGuessLoopStatus = innerGameCycleGuessLoop(currentRandomNumber, logEntryMain);
        if (innerGameCycleGuessLoopStatus == INNER_GAME_CYCLE_GUESS_LOOP_CORRECT)
        {
            isGuessedCorrect = oneGameCycle_IS_GUESSED_CORRECT;
        }
    }
    return 0; // on correct guess
}

void gameLooper(struct logEntry logEntryMain)
{
    int gameContinue = GAME_ROOT_CYCLE_CONTINUE;
    while (gameContinue == GAME_ROOT_CYCLE_CONTINUE)
    {
        oneGameCycle(logEntryMain);
        gameContinue = gameContinueCheck();
    }
}

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

int continueCheckUserInput()
{
    char bufferInputFromUser[CONTINUE_STRING_CHECK_SIZE];
    int takeInputromUserStatus = takeInputFromUser(bufferInputFromUser, sizeof(bufferInputFromUser));
    if (takeInputromUserStatus == TAKE_INPUT_FROM_USER_FAILED)
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

int takeInputFromUser(char *whereToWriteString, size_t InputStringSize)
{
    char bufferInputFromUser[InputStringSize];
    // taking user input
    // checking for input error
    if ((fgets(bufferInputFromUser, sizeof(bufferInputFromUser), stdin)) == NULL)
    {
        puts("takeInputFromUser failed");
        return TAKE_INPUT_FROM_USER_FAILED;
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
            return TAKE_INPUT_FROM_USER_FAILED;
        }
    }

    // removing the trailing newline if it exists
    bufferInputFromUser[newlineIndex] = '\0';

    // copy the new input into the target string
    strcpy(whereToWriteString, bufferInputFromUser);
    return TAKE_INPUT_FROM_USER_SUCCESS;
}

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

int generateNewRandomNumber()
{
    return rand() % (STARTING_MAX_NUMBER - STARTING_MIN_NUMBER + 1) + STARTING_MIN_NUMBER;
}

int innerGameCycleGuessLoop(int currentRandomNumber, struct logEntry logEntryMain)
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

    logEntryMain.neededNumOfGuesses = guessCounter;

    int WriteNewLogEntryStatus = WriteNewLogEntry(logEntryMain);
    if (WriteNewLogEntryStatus == WriteNewLogEntry_FAIL)
    {
        puts("WriteNewLogEntry failed in innerGameCycleGuessLoop. Could not save results");
        exit(EXIT_FAILURE);
    }

    int WriteNewTopTenEntryStatus = WriteNewTopTenEntry(logEntryMain);
    if (WriteNewTopTenEntryStatus == WriteNewTopTenEntryStatus_FAIL)
    {
        puts("WriteNewTopTenEntry failed in innerGameCycleGuessLoop.");
        exit(EXIT_FAILURE);
    }

    return INNER_GAME_CYCLE_GUESS_LOOP_CORRECT;
}

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

int prepareLogInfo(struct logEntry *logEntryMain)
{
    logEntryMain->theNumberToGuess = 0;
    logEntryMain->neededNumOfGuesses = 0;

    time(&logEntryMain->timeOfAttemptStart);

    char userName[USERNAME_ARRAY_SIZE];
    int askForUsernameStatus = askForUsername(userName);
    if (askForUsernameStatus == askForUsername_FAILED)
    {
        puts("prepareLogInfo failed because of an askForUsername error");
        return prepareLogInfo_ERROR;
    }
    strcpy(logEntryMain->userName, userName);

    return prepareLogInfo_READY;
}

int askForUsername(char *userName)
{
    int takeInputFromUserStatus = takeInputFromUser(userName, sizeof(userName));

    if (takeInputFromUserStatus == TAKE_INPUT_FROM_USER_FAILED)
    {
        puts("Programm failed to get valid input for a username!");
        return askForUsername_FAILED;
    }

    return askForUsername_SUCCESS;
}

int WriteNewLogEntry(struct logEntry logEntryMain)
{
    if (checkIfFileExists(LOGFILE_NAME) == checkIfFileExists_FALSE)
    {
        createNewFile(LOGFILE_NAME);
    }

    FILE *file = fopen(LOGFILE_NAME, "a");

    if (file == NULL)
    {
        perror("Failed to open file in WriteNewLogEntry");
        return WriteNewLogEntry_FAIL;
    }

    fprintf(file, "DATE: %s | Number of Guesses: %d | Number to guess: %d | Guessed by: %s\n",
            ctime(&logEntryMain.timeOfAttemptStart), logEntryMain.neededNumOfGuesses, logEntryMain.theNumberToGuess, logEntryMain.userName);

    fclose(file);

    return WriteNewLogEntry_SUCCESS;
}

int checkIfFileExists(char *fileNameToCheckFor)
{
    FILE *file = fopen(fileNameToCheckFor, "r");

    if (file == NULL)
    {
        printf("File &s found.");
        return checkIfFileExists_FALSE;
    }
    else
    {
        puts("File exists and can be opened.");
        fclose(file);
    }

    return checkIfFileExists_TRUE;
}

int createNewFile(char *newFileName)
{
    FILE *file = fopen(newFileName, "w");

    if (file == NULL)
    {
        perror("Failed to create file");
        return createNewFile_FAIL;
    }

    fclose(file);

    return createNewFile_SUCCESS;
}

int WriteNewTopTenEntry(struct logEntry logEntryMain)
{
    int TopEntryFileExists = checkIfcheckIfFileExists(TOP_TEN_NAME);
    if (TopEntryFileExists = checkIfFileExists_FALSE)
    {
        createNewFile(TOP_TEN_NAME);
    }

    // adding a new entry
    FILE *file = fopen("bestenliste.txt", "r");

    if (file == NULL)
    {
        perror("Failed to open file in WriteNewTopEntry");
        return EXIT_FAILURE;
    }

    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        printf("%s", buffer);
    }

    fclose(file);

    return WriteNewTopTenEntryStatus_FAIL;

    return WriteNewTopTenEntryStatus_SUCCESS;
}