/*
Aufgabe 2.5

In diesem Beispiel soll das ” Spiel“ Zahlenraten implementiert werden.

1. Der Computer erzeugt eine Zufallszahl zwischen 1 und 100, die es zu erraten gilt. Die Spielerin wird solange aufgefordert eine Zahl einzugeben (zu raten), bis die Zahl erraten wurde. Nach jeder Eingabe gibt der Computer an, ob die Zufallszahl kleiner, gr¨ oßer oder gleich der geratenen Zahl ist. Das Spiel ist beendet, sobald die Zahl erraten wurde. Die Anzahl der Rateversuche wird mitgez¨ ahlt und am Spielende ausgegeben. Nach jedem Spiel wird die Spielerin gefragt, ob noch eine Runde gespielt werden soll. Wird diese Abfrage verneint, wird das Programm beendet.

2. DasZahlenraten soll nun um eine Bestenliste (der besten 10), die in einer Datei abgespeichert ist, erweitert werden.
Die Eintr¨ age in der Bestenliste sind nach der Anzahl der Rateversuche geordnet. Nummer 1 in der Bestenliste hat die wenigsten Versuche gebraucht, um eine Zahl korrekt zu raten. Bei Gleichstand wird der alte Eintrag nach unten verschoben (der neue Eintrag vor dem ¨ alteren eingef¨ ugt). Um einen ¨ Uberblick ¨ uber die abgeschlossenen Spiele zu haben, wird in eine Logdatei die jeweils zu erratende Zahl und die Anzahl der ben¨ otigten Rateversuche protokolliert.
*/

// includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// definitions
#define NAME_MAX_CHAR_COUNT 20
#define NAME_CAPACITY_NEEDED (NAME_MAX_CHAR_COUNT + 1)
#define MAX_NUMBER_RANGE 100
#define MIN_NUMBER_RANGE 1
#define MAX_DIGITS_TO_GUESS 3

// constants
char KEYWORD_TO_CONTINUE[] = "yes";

// declarations
int setPlayerName(char *playerNameAddress, size_t capacity);
int generateRandomNumber(void);
void gameCycleLoop(void);
int captureCleanInputFromUser(char *whereToWriteString, size_t writingCapicity);
int guessingGameCycle(int *guessAttemptsCounter, int *currentMin, int *currentMax, int numberToGuess);
int continueCheck(void);

// =======================================
// Start of main
int main(void)
{
    puts("Number Guesser game started!");
    srand(time(0)); // initialize random seed once only

    char playerName[NAME_CAPACITY_NEEDED];
    if (!setPlayerName(playerName, sizeof(playerName)))
    {
        puts("could not read the username input. Exiting programm.");
        return 1;
    }

    printf("The player name is %s.\n", playerName);
    puts("Starting game loop:....");

    gameCycleLoop();

    return 0;
}

// =======================================
// Help functions
int setPlayerName(char *playerNameAddress, size_t capacity)
{
    puts("Please type the player name:");

    return captureCleanInputFromUser(playerNameAddress, capacity);
}

int generateRandomNumber(void)
{
    int generatedNumber = rand() % (MAX_NUMBER_RANGE - MIN_NUMBER_RANGE + 1) + MIN_NUMBER_RANGE;
    return generatedNumber;
}

void gameCycleLoop(void)
{
    int isContinueSelected = 1;
    while (isContinueSelected)
    {
        int numberToGuess = generateRandomNumber();
        puts("Number to guess has been generated.");

        int currentMin = MIN_NUMBER_RANGE;
        int currentMax = MAX_NUMBER_RANGE;

        int isNumberGuessedCorrectly = 0;
        int guessAttemptsCounter = 0;
        while (!isNumberGuessedCorrectly)
        {
            isNumberGuessedCorrectly = guessingGameCycle(&guessAttemptsCounter, &currentMin, &currentMax, numberToGuess);
        }

        isContinueSelected = continueCheck();
    }
    puts("User chose not to continue guessing");
}

int captureCleanInputFromUser(char *whereToWriteString, size_t writingCapicity)
{
    if (fgets(whereToWriteString, writingCapicity, stdin) == NULL)
    {
        return 0; // input failed
    }
    else if (whereToWriteString[writingCapicity + 1] != '\0')
    {
        printf("Input was too long. Please input only %d.\n", writingCapicity);
    }

    // Remove the trailing newline, if present
    whereToWriteString[strcspn(whereToWriteString, "\n")] = '\0';

    return 1; // input success
}

int guessingGameCycle(int *guessAttemptsCounter, int *currentMin, int *currentMax, int numberToGuess)
{
    (*guessAttemptsCounter)++;
    printf("The number is between %d and %d. Please enter your guess:\n", *currentMin, *currentMax);
    int currentGuess = 0;
    char bufferGuess[MAX_DIGITS_TO_GUESS + 1];

    if (!captureCleanInputFromUser(bufferGuess, MAX_DIGITS_TO_GUESS + 1))
    {
        puts("Could not read the input number, try again");
        return 0; // failed input
    }

    if (!convertFromStringToInteger(bufferGuess, currentGuess))
    {
        puts("Could not read the input number, try again");
        return 0; // failed input
    }

    if (scanf("%d", &currentGuess) <= 0)
    {
        printf("invalid guess. Try again by inputting a number between %d and %d.\n", *currentMin, *currentMax);
    }
    if (currentGuess == numberToGuess)
    {
        printf("You got it! %d is right!\n You guessed the correct answer in %d attempts!\n", numberToGuess, *guessAttemptsCounter);
        return 1; // successful guess
    }
    else if (currentGuess < numberToGuess)
    {
        puts("Your guess is too low. Guess higher.");
        if (currentGuess > *currentMin)
        {

            *currentMin = currentGuess + 1;
        }
    }
    else if (currentGuess > numberToGuess)
    {
        puts("Your guess is too high. Guess lower.");
        if (currentGuess < *currentMax)
        {
            *currentMax = currentGuess - 1;
        }
    }
    return 0;
}

int continueCheck(void)
{
    printf("Do you want another random number to guess? Type '%s' to continue.\n", KEYWORD_TO_CONTINUE);
    char userContinueAnswer[32] = {0};
    captureCleanInputFromUser(userContinueAnswer, sizeof(userContinueAnswer));
    if (strcmp(userContinueAnswer, KEYWORD_TO_CONTINUE) == 0)
    {
        return 1;
    }
    return 0;
}

int convertFromStringToInteger(char sourceString, int *targetInt)
{
    char *endptr;
    int convertedNumber = strtol(sourceString, &endptr, 10);
    if (endptr == sourceString)
    {
        printf("Invalid: No number found\n");
        return 0;
    }
    else if (*endptr != '\0')
    {
        printf("Invalid: Extra characters\n");
        return 0;
    }
    else
    {
        *targetInt = convertedNumber;
        return 1;
    }
}