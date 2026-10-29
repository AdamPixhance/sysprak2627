#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// defining const strings
const char Matrikelnummer[] = "12499672";
const char DefaultName[] = "Adam Alkhatib";

// 1 print
void print(char *eingabetext)
{
    printf("%s\n", eingabetext);
}

// 2 reverse
void reverse(char *eingabetext)
{
    size_t size = strlen(eingabetext);
    char reversed[size + 1];
    for (int i = size - 1; i >= 0; i--)
    {
        reversed[size - i - 1] = eingabetext[i];
    }
    reversed[size] = '\0';
    strcpy(eingabetext, reversed);
}

// 3 duplicate
char *duplicate(char *eingabetext)
{
    // allocate temp memory with malloc with double the size of eingabetext
    char *temp = realloc(eingabetext, 2 * strlen(eingabetext) + 1);
    if (temp == NULL) // check for error
    {
        perror("realloc on *temp failed in 3 duplicate");
        exit(1);
    } // if no error, then copy it into eingabetext

    // now duplicate
    // first create an empty string with the correct size
    char bufferString[strlen(temp) + 1];

    // copy the original eingabe in that buffer
    strcpy(bufferString, temp);

    // str cat that buffer to our eingabe text
    strcat(temp, bufferString);

    // return the new pointer of eingabetext
    return temp;
}

// 4 increment
void increment(int jump, char *eingabetext)
{
    size_t length = strlen(eingabetext);
    for (int i = 0; i <= length; i++)
    {
        if (eingabetext[i] >= 65 && eingabetext[i] <= 90) // capital letters first
        {
            eingabetext[i] += jump;
            if (eingabetext[i] > 90) // correcting for letters that go past Z
            {
                eingabetext[i] -= 26;
            }
        }
        else if (eingabetext[i] >= 97 && eingabetext[i] <= 122) // small case letters
        {
            eingabetext[i] += jump;
            if (eingabetext[i] > 122) // correcting for letters jumping past z
            {
                eingabetext[i] -= 26;
            }
        }
    }
}

// 5 decrement
void decrement(int jump, char *eingabetext)
{
    size_t length = strlen(eingabetext);
    for (int i = 0; i <= length; i++)
    {
        if (eingabetext[i] >= 65 && eingabetext[i] <= 90) // capital letters first
        {
            eingabetext[i] -= jump;
            if (eingabetext[i] < 65) // correcting for letters that go behind the A
            {
                eingabetext[i] += 26;
            }
        }
        else if (eingabetext[i] >= 97 && eingabetext[i] <= 122) // small case letters
        {
            eingabetext[i] -= jump;
            if (eingabetext[i] < 97) // correcting for letters jumping behind the a
            {
                eingabetext[i] += 26;
            }
        }
    }
}

// 6 count letters
countLetters

// ---------------------------------------------------
// start of main
int main(int argc, char *argv[])
{
    // Determine the input string
    const char *source;

    if (argc > 1) // if the user gave any arguemnts
    {
        source = argv[1];
    }
    else // no arguements given
    {
        source = DefaultName;
    }

    // Calculate required memory
    size_t size = strlen(source);

    // Allocate memory with malloc
    char *dynamicInput = malloc(size + 1);

    if (dynamicInput == NULL)
    {
        perror("malloc failed");
        return 1;
    }

    // Copy string into the allocated memory
    strcpy(dynamicInput, source);

    // ----------------------------------
    // start using the functions
    // 1 testing print
    puts("Testing print:");
    print(dynamicInput);

    // 2 testing reverse
    puts("\nTesting reverse:");
    reverse(dynamicInput);
    print(dynamicInput);

    // 3 testing duplicate
    puts("\nTesting duplicate:");
    dynamicInput = duplicate(dynamicInput);
    print(dynamicInput);

    // 4 testing increment
    puts("\nTesting increment:");
    increment(5, dynamicInput);
    print(dynamicInput);

    // 5 testing decrement
    puts("\nTesting decrement:");
    decrement(5, dynamicInput);
    print(dynamicInput);

    // 6 count letters
    puts("\nTesting count letters:");


    // End of main
    free(dynamicInput);
    dynamicInput = NULL;
    return 0;
}