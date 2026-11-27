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
#include <stdbool.h>
#include <errno.h>
#include <limits.h>

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
#define FILE_TOP_TEN "numberguesser_highscores.txt"
#define FILE_TOP_TEN_TEMP "numberguesser_highscores.tmp"
#define TOP_TEN_MAX_ENTRIES 10
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

// writeNewTopTenEntryStatus constants
#define writeNewTopTenEntryStatus_SUCCESS 0
#define writeNewTopTenEntryStatus_FAIL 1
#define NUMBER_OF_GUESSES_KEYWORD_WITH_TRAILING_WHITESPACE "Number of Guesses: "

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
int askForUsername(char *userName, size_t userNameSize);
int WriteNewLogEntry(struct logEntry logEntryMain);
int checkIfFileExists(char *fileNameToCheckFor);
int createNewFile(char *newFileName);
int writeNewTopTenEntry(struct logEntry logEntryMain);
int writeLogEntryToFile(FILE *file, struct logEntry logEntryMain);

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
    // Record the start of this round, not just the start of the program.
    if (time(&logEntryMain.timeOfAttemptStart) == (time_t)-1)
    {
        puts("Failed to obtain the current time");
        exit(EXIT_FAILURE);
    }
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

    int writeNewTopTenEntryStatus = writeNewTopTenEntry(logEntryMain);
    if (writeNewTopTenEntryStatus == writeNewTopTenEntryStatus_FAIL)
    {
        puts("Could not update the highscores file. The game was saved in the log.");
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

    logEntryMain->timeOfAttemptStart = 0; // Set at the start of each round.

    char userName[USERNAME_ARRAY_SIZE];
    int askForUsernameStatus = askForUsername(userName, sizeof(userName));
    if (askForUsernameStatus == askForUsername_FAILED)
    {
        puts("prepareLogInfo failed because of an askForUsername error");
        return prepareLogInfo_ERROR;
    }
    strcpy(logEntryMain->userName, userName);

    return prepareLogInfo_READY;
}

int askForUsername(char *userName, size_t userNameSize)
{
    puts("Please enter your username:");
    int takeInputFromUserStatus = takeInputFromUser(userName, userNameSize);

    if (takeInputFromUserStatus == TAKE_INPUT_FROM_USER_FAILED)
    {
        puts("Programm failed to get valid input for a username!");
        return askForUsername_FAILED;
    }

    return askForUsername_SUCCESS;
}

// Writes exactly one entry to an already opened file.
// The calling function is responsible for opening and closing that file.
int writeLogEntryToFile(FILE *file, struct logEntry logEntryMain)
{
    char *dateString = ctime(&logEntryMain.timeOfAttemptStart);
    if (dateString == NULL)
    {
        puts("Could not convert the date");
        return WriteNewLogEntry_FAIL;
    }

    // %.24s omits the trailing newline in the usual ctime() string.
    if (fprintf(file, "DATE: %.24s | %s%d | Number to guess: %d | Guessed by: %s\n",
                dateString, NUMBER_OF_GUESSES_KEYWORD_WITH_TRAILING_WHITESPACE,
                logEntryMain.neededNumOfGuesses, logEntryMain.theNumberToGuess,
                logEntryMain.userName) < 0)
    {
        perror("Failed to write log entry");
        return WriteNewLogEntry_FAIL;
    }

    return WriteNewLogEntry_SUCCESS;
}

int WriteNewLogEntry(struct logEntry logEntryMain)
{
    // Append mode creates the log if necessary and preserves existing entries.
    FILE *file = fopen(LOGFILE_NAME, "a");
    if (file == NULL)
    {
        perror("Failed to open file in WriteNewLogEntry");
        return WriteNewLogEntry_FAIL;
    }

    int status = writeLogEntryToFile(file, logEntryMain);
    if (fclose(file) == EOF)
    {
        perror("Failed to close log file");
        return WriteNewLogEntry_FAIL;
    }

    return status;
}

int checkIfFileExists(char *fileNameToCheckFor)
{
    FILE *file = fopen(fileNameToCheckFor, "r");

    if (file == NULL)
    {
        printf("Could not open file %s.\n", fileNameToCheckFor);
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

int writeNewTopTenEntry(struct logEntry logEntryMain)
{
    // a+ creates an empty file if needed, without erasing an existing file.
    // We only READ from this stream; all changes go into the temporary file.
    FILE *file = fopen(FILE_TOP_TEN, "a+");
    if (file == NULL)
    {
        perror("Failed to open highscores file");
        return writeNewTopTenEntryStatus_FAIL;
    }

    // Explicitly move to the start before reading.
    if (fseek(file, 0, SEEK_SET) != 0)
    {
        perror("Failed to move to the beginning of the highscores file");
        fclose(file);
        return writeNewTopTenEntryStatus_FAIL;
    }

    FILE *temporaryFile = fopen(FILE_TOP_TEN_TEMP, "w");
    if (temporaryFile == NULL)
    {
        perror("Failed to open temporary highscores file");
        fclose(file);
        return writeNewTopTenEntryStatus_FAIL;
    }

    char buffer[256];
    size_t lineTracker = 0;
    size_t linesWritten = 0;
    bool newEntryHasBeenInserted = false;
    bool processingFailed = false;
    const char *keywordToLookFor = NUMBER_OF_GUESSES_KEYWORD_WITH_TRAILING_WHITESPACE;

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        lineTracker++;

        // Every entry written by writeLogEntryToFile ends with a newline.
        // Reject incomplete/oversized lines instead of rewriting corrupt data.
        if (strchr(buffer, '\n') == NULL)
        {
            fprintf(stderr, "Incomplete or oversized entry on line %zu\n", lineTracker);
            processingFailed = true;
            break;
        }

        // 1. Find the number of guesses in the current line.
        char *foundPosition = strstr(buffer, keywordToLookFor);
        if (foundPosition == NULL)
        {
            fprintf(stderr, "Keyword not found on line %zu\n", lineTracker);
            processingFailed = true;
            break;
        }
        foundPosition += strlen(keywordToLookFor);

        // 2. Convert the number and validate it and its following separator.
        char *endptr;
        errno = 0;
        long guessesFromFile = strtol(foundPosition, &endptr, 10);
        if (endptr == foundPosition || errno == ERANGE ||
            guessesFromFile < 1 || guessesFromFile > INT_MAX ||
            strncmp(endptr, " | Number to guess: ", strlen(" | Number to guess: ")) != 0)
        {
            fprintf(stderr, "Invalid number of guesses on line %zu\n", lineTracker);
            processingFailed = true;
            break;
        }

        // 3. Insert the new entry BEFORE the first equal or worse old entry.
        if (!newEntryHasBeenInserted &&
            guessesFromFile >= logEntryMain.neededNumOfGuesses &&
            linesWritten < TOP_TEN_MAX_ENTRIES)
        {
            if (writeLogEntryToFile(temporaryFile, logEntryMain) != WriteNewLogEntry_SUCCESS)
            {
                processingFailed = true;
                break;
            }
            linesWritten++;
            newEntryHasBeenInserted = true;
        }

        // 4. Copy the existing entry, but never write more than ten entries.
        if (linesWritten < TOP_TEN_MAX_ENTRIES)
        {
            if (fputs(buffer, temporaryFile) == EOF)
            {
                perror("Failed to copy existing highscore");
                processingFailed = true;
                break;
            }
            linesWritten++;
        }
    }

    // NULL from fgets() can mean a read error, not just EOF.
    if (ferror(file))
    {
        perror("Failed to read highscores file");
        processingFailed = true;
    }

    // 5. If not inserted yet, append only if there is still room.
    if (!processingFailed && !newEntryHasBeenInserted && linesWritten < TOP_TEN_MAX_ENTRIES)
    {
        if (writeLogEntryToFile(temporaryFile, logEntryMain) != WriteNewLogEntry_SUCCESS)
        {
            processingFailed = true;
        }
        else
        {
            linesWritten++;
            newEntryHasBeenInserted = true;
        }
    }

    // 6. Close both streams even if one operation fails.
    if (fclose(file) == EOF)
    {
        perror("Failed to close original highscores file");
        processingFailed = true;
    }
    if (fclose(temporaryFile) == EOF)
    {
        perror("Failed to close temporary highscores file");
        processingFailed = true;
    }

    // Never replace the original with an incomplete temporary file.
    if (processingFailed)
    {
        if (remove(FILE_TOP_TEN_TEMP) != 0)
        {
            perror("Failed to remove incomplete temporary file");
        }
        return writeNewTopTenEntryStatus_FAIL;
    }

    // 7. On Linux, rename() replaces the original file if it exists.
    if (rename(FILE_TOP_TEN_TEMP, FILE_TOP_TEN) != 0)
    {
        perror("Failed to replace highscores file");
        return writeNewTopTenEntryStatus_FAIL;
    }

    return writeNewTopTenEntryStatus_SUCCESS;
}
