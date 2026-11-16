/*
Aufgabe 1.4: gameOfLife.c
Implementieren Sie das game of life. Die Gr¨ oße des Spielfeldes soll 16 × 16 Felder betragen, optional soll der Anwen
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
#include <unistd.h>

#define DEFAULT_SIZE 16

int main(int argc, char *argv[])
{
    int opt;
    int gridSize = DEFAULT_SIZE;
    int density = 0;

    // Remember whether -d was provided
    int densityProvided = 0;

    while ((opt = getopt(argc, argv, ":d:s:")) != -1)
    {
        switch (opt)
        {
        case 'd':
        {
            int tmpD = atoi(optarg);

            if (tmpD < 0 || tmpD > 100)
            {
                puts("Density must be between 0 and 100!");
                return 1;
            }

            density = tmpD;
            densityProvided = 1;
            break;
        }

        case 's':
        {
            int tmpS = atoi(optarg);

            if (tmpS < 4)
            {
                puts("Grid size must be >= 4!");
                return 1;
            }

            gridSize = tmpS;
            break;
        }

        case ':':
            puts("Error: An option is missing its value!");
            return 1;

        case '?':
        default:
            puts("Error: Unknown option!");
            return 1;
        }
    }

    // Check that the mandatory option was provided
    if (!densityProvided)
    {
        puts("Error: You must specify density using -d!");
        return 1;
    }

    printf("Grid size: %d x %d\n", gridSize, gridSize);
    printf("Density: %d%%\n", density);

    return 0;
}