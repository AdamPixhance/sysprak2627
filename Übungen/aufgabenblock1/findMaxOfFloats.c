/*
Aufgabe 1.3
Gegeben ist eine Datei, die beliebig viele Zahlen des Typs float enth¨ alt, wobei je Zeile genau eine Zahl enthalten
ist. Schreiben Sie ein Programm, das eine solche Datei ¨ offnet und die darin enthaltenen Zahlen in einem Feld/Array
speichert. Der Dateiname der zu ¨ offnenden Datei soll dabei entweder als Kommandozeilenparameter ¨ ubergeben werden
k¨ onnen, oder, falls kein Kommandozeilenparameter ¨ ubergeben wurde, im Programmverlauf abgefragt werden. Bestim
men Sie anschließend das Maximum der gelesenen Zahlen und geben Sie es bis auf zwei Stellen nach dem Komma genau
auf der Standardausgabe aus.
Hinweis: Sie finden eine Datei mit 10.000 Zufallszahlen zu Testzwecken im Umfang der Beispielimplementierung ent
halten.

Inhalte, die dieses Beispiel vermitteln soll:
- Speicherverwaltung
- Felder/Arrays (Buch1: 11)
- Dateiein- und-ausgabe (Buch1: 16)
- Formatierte Ein- und Ausgabe (Buch1: 4.2)
- Kontrollstrukturen (Bedingungen, Schleifen) (Buch1: 8)
*/

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

// beginning of main
int main(int argc, char *argv[])
{
    /*
    We want to create a  new pointer with the type FILE
    then we will use fopen to assign a value to that pointer
    fopen then returns an address to acces our target file
    "r" here is our fopen mode, which is reading mode
    */

    int option;          // needed to go through the argc options
    FILE *myFile = NULL; // declaring an empty FILE pointer
    if ((option = getopt(argc, argv, "f:")) != -1)
    {
        switch (option)
        {
        case 'f': // the case where the user types the correct command
            myFile = fopen(optarg, "r");
            break;

        default:
            puts("invalid option given. Please type ./find max -f \"<your file path and name>\" correctly and try again.");
            exit(1);
        }
    }
    else
    {
        puts("Please write the name of the file you want to process:");
        char *bufferFileName;
        scanf("%s", &bufferFileName);
        myFile = fopen(*bufferFileName, "r");
    }

    // error handling in the case of the file not opening
    if (myFile == NULL)
    {
        perror("fopen failed to open the given file"); // perror gives more context if available from the stdlib
        exit(1);
    }
    else // small update string for debugging
    {
        puts("File open = success! Looking for the biggest number in the given file ...");
    }

    // maxDobule will store the result
    double maxDouble = 0;

    {                             // creating a local scope
                                  // create buffers to store the floats we read
        char bufferChar[40] = {}; // Googled how many digits are needed to type out that max float in C
        double bufferDouble = 0;
        int counter = 0;
        // using fgets we can read until the first encountered /n
        // with the while loop, we keep doing it until we get back NULL
        while (fgets(bufferChar, sizeof(bufferChar), myFile) != NULL)
        {
            if (counter == 0) // making sure to take at least 1 input as my max. Because the  max in the file can be already smaller than 0.
            {
                maxDouble = bufferDouble; // regaradelss of what the first Double is, it will always be the max.
                counter = 1;
            }
            else // the normal for loop side
            {
                // converting that bufferChar into a float using strtof
                bufferDouble = strtod(bufferChar, NULL);

                if (maxDouble < bufferDouble)
                {
                    // passing over from my buffer double to my maxDouble
                    maxDouble = bufferDouble;
                }
            }
        }
    }

    // Result of the max found double
    printf("The max float found in the file is: %.2f\n", maxDouble);

    // close the file at the end
    fclose(myFile);

    // end of main!
    puts("\n----------------------------\nReturning 0 to end the programm.\nBye Bye.");
    return 0;
}