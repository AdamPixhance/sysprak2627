/*
Aufgabe 1.4: gameOfLife.c
Implementieren Sie das game of life. Die Gr¨ oße des Spielfeldes soll 16 × 16 Felder betragen, optionsIncional soll der Anwen
der per Kommandozeilenparameter eine alternative Gr¨ oße des Spielfeldes angeben k¨ onnen. Die initiale Belegung des
Spielfeldes mit lebenden Zellen soll zuf¨ allig geschehen, wobei obligatorisch die Dichte der Belegung des Spielfeldes mit
lebenden Zellen per Kommandozeilenparameter vom Nutzer angegeben werden muss. Geben Sie die erzeugte Popula
tion und jeweils mit einer Sekunde Verz¨ ogerung die Population der n¨ achsten Spielrunde auf der Standardausgabe aus.
Stagniert eine Population, d.h. es gibt von einer Runde zur n¨ achsten keine ¨ Anderung, soll das Programm terminieren.

Hinweis: Regeln des game of life
- Eine tote Zelle mit genau drei lebenden Nachbarn wird in der Folgegeneration neu geboren.
- Lebende Zellen mit weniger als zwei lebenden Nachbarn sterben in der Folgegeneration an Einsamkeit.
- Eine lebende Zelle mit zwei oder drei lebenden Nachbarn bleibt in der Folgegeneration lebend.
- Lebende Zellen mit mehr als drei lebenden Nachbarn sterben in der Folgegeneration an ¨ Uberbev¨ olkerung.
- Hinweis: alle anderen Zellen leben nicht
*/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_SIZE 16
#define ALIVE '#'
#define DEAD ' '
#define STEP_LINE_BREAKER "==================================================="

void populateNewGame(int gridSize, int density, char gameArray[gridSize][gridSize]);
int populateBasedOnDensity(int density);
void printGameState(int gridSize, char gameArray[gridSize][gridSize]);
int isCellAlive(char cellToCheck);
int countAliveNeighbours(int row, int column, int gridSize, char gameArray[gridSize][gridSize]);
int isNextStepStalled(int gridSize, char gameArray[gridSize][gridSize]);

int main(int argc, char *argv[])
{

    int gridSize = DEFAULT_SIZE;
    int density = 0;

    // Remember whether -d was provided
    int densityProvided = 0;

    {
        int optionsInc;
        while ((optionsInc = getopt(argc, argv, "d:s:")) != -1)
        {
            switch (optionsInc)
            {
            case 'd':
            {
                int bufferDensity = -1;
                bufferDensity = strtol(optarg, NULL, 10);

                if (bufferDensity < 0)
                {
                    puts("Density must be between 0 and 100!");
                    return 1;
                }
                else if (bufferDensity >= 100)
                {
                    if (bufferDensity > 100)
                    {
                        puts("Density values larger than 100 default down to 100\%");
                        bufferDensity = 100;
                    }
                    puts("Warning: 100\% density gives the same boring result everytime!");
                }

                density = bufferDensity;
                densityProvided = 1;
                break;
            }

            case 's':
            {
                int bufferSize = strtol(optarg, NULL, 10);

                if (bufferSize <= 0)
                {
                    puts("Grid size must be > 0!");
                    return 1;
                }

                gridSize = bufferSize;
                break;
            }
            }
        }
    }

    // Check that the mandatory density was provided
    if (!densityProvided)
    {
        puts("Error: You must specify density using -d!");
        return 1;
    }

    puts("Starting the game of life with the current parameters:");
    printf("Grid size: %d x %d\n", gridSize, gridSize);
    printf("Density: %d%%\n", density);

    // Start of the game
    char gameArray[gridSize][gridSize];
    populateNewGame(gridSize, density, gameArray);
    printGameState(gridSize, gameArray);

    int gameStalled = 0;
    int stepCounter = 1;

    while (!gameStalled)
    {
        sleep(1);
        gameStalled = isNextStepStalled(gridSize, gameArray);
        printf("\n\n%s\n@Step:%d\n\n", STEP_LINE_BREAKER, stepCounter);
        printGameState(gridSize, gameArray);
        stepCounter += 1;
    }

    printf("\n %s\nTHE END\n\nThe game of life reached its end. The cells within are not changing anymore. Thanks for running the game!\n", STEP_LINE_BREAKER);

    return 0;
}

void populateNewGame(int gridSize, int density, char gameArray[gridSize][gridSize])
{
    srand(time(NULL)); // this generates a random seed for the RNG using the current time
    for (int i = 0; i < gridSize; i++)
    {
        for (int j = 0; j < gridSize; j++)
        {
            if (populateBasedOnDensity(density) == 1)
            {
                gameArray[i][j] = ALIVE;
            }
            else
            {
                gameArray[i][j] = DEAD;
            }
        }
    }
}

int populateBasedOnDensity(int aliveChance)
{
    if ((rand() % (100)) < aliveChance)
    {
        return 1;
    }
    return 0;
}

void printGameState(int gridSize, char gameArray[gridSize][gridSize])
{
    for (int i = 0; i < gridSize; i++)
    {
        // Horizontal border
        for (int j = 0; j < gridSize; j++)
        {
            printf("+------");
        }
        printf("+\n");

        // Row contents
        printf("|");
        for (int j = 0; j < gridSize; j++)
        {
            printf("  %c   |", gameArray[i][j]);
        }
        printf("\n");
    }

    // Bottom border
    for (int i = 0; i < gridSize; i++)
    {
        printf("+------");
    }
    printf("+\n");
}

int isCellAlive(char cellToCheck)
{
    return (cellToCheck == ALIVE);
}

int countAliveNeighbours(int row, int column, int gridSize, char gameArray[gridSize][gridSize])
{
    int gridBorder = gridSize - 1;
    int aliveCounter = 0;

    for (int i = row - 1; i <= row + 1; i++)
    {
        for (int j = column - 1; j <= column + 1; j++)
        {
            // Skip coordinates outside the grid
            if (i < 0 || i > gridBorder ||
                j < 0 || j > gridBorder)
            {
                continue;
            }

            // Skip the cell itself
            if (i == row && j == column)
            {
                continue;
            }

            if (isCellAlive(gameArray[i][j]))
            {
                aliveCounter++;
            }
        }
    }

    return aliveCounter;
}

int isNextStepStalled(int gridSize, char gameArray[gridSize][gridSize])
{
    char bufferImageOfGameArray[gridSize][gridSize];
    memcpy(bufferImageOfGameArray, gameArray, sizeof(bufferImageOfGameArray));
    int changecounter = 0;

    for (int i = 0; i < gridSize; i++)
    {
        for (int j = 0; j < gridSize; j++)
        {
            int isBufferAlive = isCellAlive(bufferImageOfGameArray[i][j]);
            int bufferAliveCounter = countAliveNeighbours(i, j, gridSize, bufferImageOfGameArray);

            if (!isBufferAlive && bufferAliveCounter == 3)
            {
                gameArray[i][j] = ALIVE;
            }
            else if (isBufferAlive && bufferAliveCounter < 2)
            {
                gameArray[i][j] = DEAD;
            }
            else if (isBufferAlive &&
                     (bufferAliveCounter == 2 || bufferAliveCounter == 3))
            {
                gameArray[i][j] = ALIVE;
            }
            else if (isBufferAlive && bufferAliveCounter > 3)
            {
                gameArray[i][j] = DEAD;
            }
            else
            {
                gameArray[i][j] = DEAD;
            }

            // check if anything changed
            if (gameArray[i][j] != bufferImageOfGameArray[i][j])
            {
                changecounter += 1;
            }
        }
    }
    if (changecounter > 0)
    {
        return 0; // game is still changing
    }
    else
    {
        return 1; // game stalled
    }
}