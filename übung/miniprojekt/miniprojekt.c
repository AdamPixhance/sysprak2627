#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// defining const strings
const char Matrikelnummer[] = "12499672";
const char DefaultName[] = "Adam Alkhatib";

// 1 - print
void print(char *eingabetext)
{
    printf("%s\n", eingabetext);
}

// 2 - reverse
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

// 3 - duplicate
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

// 4 - increment
void increment(int jump, char *eingabetext)
{
    size_t length = strlen(eingabetext);
    for (size_t i = 0; i < length; i++)
    {
        if (eingabetext[i] >= 65 && eingabetext[i] <= 90) // capital letters first
        {
            int newChar = (unsigned char)eingabetext[i] + jump;
            if (newChar > 90) // correcting for letters that go past Z
            {
                newChar -= 26;
            }
            eingabetext[i] = newChar;
        }
        else if (eingabetext[i] >= 97 && eingabetext[i] <= 122) // small case letters
        {
            int newChar = (unsigned char)eingabetext[i] + jump;
            if (newChar > 122) // correcting for letters jumping past z
            {
                newChar -= 26;
            }
            eingabetext[i] = newChar;
        }
    }
}

// 5 - decrement
void decrement(int jump, char *eingabetext)
{
    size_t length = strlen(eingabetext);
    for (size_t i = 0; i < length; i++)
    {
        if (eingabetext[i] >= 65 && eingabetext[i] <= 90) // capital letters first
        {
            int newChar = (unsigned char)eingabetext[i] - jump;
            if (newChar < 65) // correcting for letters that go behind the A
            {
                newChar += 26;
            }
            eingabetext[i] = newChar;
        }
        else if (eingabetext[i] >= 97 && eingabetext[i] <= 122) // small case letters
        {
            int newChar = (unsigned char)eingabetext[i] - jump;
            if (newChar < 97) // correcting for letters jumping behind the a
            {
                newChar += 26;
            }
            eingabetext[i] = newChar;
        }
    }
}

// 6 - count letters
char *countLetters(char *eingabetext)
{
    // measure length
    size_t eingabeLength = strlen(eingabetext);

    // create the counter array and fill it with empty values
    size_t counterCapacity = eingabeLength == 0 ? 1 : eingabeLength;
    size_t counter[counterCapacity][2];
    // fill counter with empty values
    for (size_t i = 0; i < counterCapacity; i++)
    {
        counter[i][0] = 0;
        counter[i][1] = 0;
    }

    // count how often each character appears
    size_t uniqueCount = 0;
    for (size_t i = 0; i < eingabeLength; i++)
    {
        int found = 0;
        for (size_t j = 0; j < uniqueCount; j++)
        {
            if (counter[j][0] == (unsigned char)eingabetext[i])
            {
                counter[j][1] += 1;
                found = 1;
                break;
            }
        }

        if (!found)
        {
            counter[uniqueCount][0] = (unsigned char)eingabetext[i];
            counter[uniqueCount][1] = 1;
            uniqueCount++;
        }
    }

    // measure the space needed for each character and its count
    size_t outputLength = 0;
    for (size_t i = 0; i < eingabeLength; i++)
    {
        for (size_t j = 0; j < uniqueCount; j++)
        {
            if (counter[j][0] == (unsigned char)eingabetext[i])
            {
                size_t count = counter[j][1];
                size_t digitCount = 1;
                while (count >= 10)
                {
                    count /= 10;
                    digitCount++;
                }
                outputLength += 1 + digitCount;
                break;
            }
        }
    }

    // create bufferText with enough space for the counted text
    char *bufferText = malloc(outputLength + 1);
    if (bufferText == NULL)
    {
        perror("malloc failed in countLetters");
        exit(1);
    }

    // add each original character and its total count to bufferText
    size_t currentBufferLength = 0;
    for (size_t i = 0; i < eingabeLength; i++)
    {
        size_t j = 0;
        while (counter[j][0] != (unsigned char)eingabetext[i])
        {
            j++;
        }

        bufferText[currentBufferLength] = eingabetext[i];
        currentBufferLength++;
        int written = snprintf(bufferText + currentBufferLength,
                               outputLength - currentBufferLength + 1,
                               "%zu",
                               counter[j][1]);
        if (written < 0)
        {
            free(bufferText);
            perror("snprintf failed in countLetters");
            exit(1);
        }
        currentBufferLength += (size_t)written;
    }
    bufferText[currentBufferLength] = '\0';

    // resize the input string after bufferText has been filled
    char *temp = realloc(eingabetext, outputLength + 1);
    if (temp == NULL)
    {
        free(bufferText);
        perror("realloc failed in countLetters");
        exit(1);
    }

    // copy the counted text into the resized input string and return it
    strcpy(temp, bufferText);
    free(bufferText);
    return temp;
}

// 7 - snakeify
char *snakeify(char *eingabetext)
{
    // measure the input length
    size_t eingabeLength = strlen(eingabetext);

    // return the input if it is empty
    if (eingabeLength == 0)
    {
        return eingabetext;
    }

    // create an empty bufferText with enough space for the result
    char bufferText[eingabeLength * 2];
    strcpy(bufferText, "");

    // add each character and an underscore, stopping before the last character
    for (size_t i = 0; i < eingabeLength - 1; i++)
    {
        size_t currentBufferLength = strlen(bufferText);
        bufferText[currentBufferLength] = eingabetext[i];
        bufferText[currentBufferLength + 1] = '_';
        bufferText[currentBufferLength + 2] = '\0';
    }

    // add the last character without an underscore after it
    size_t currentBufferLength = strlen(bufferText);
    bufferText[currentBufferLength] = eingabetext[eingabeLength - 1];
    bufferText[currentBufferLength + 1] = '\0';

    // resize the input string to make room for bufferText
    char *temp = realloc(eingabetext, eingabeLength * 2);
    if (temp == NULL)
    {
        perror("realloc failed in snakeify");
        exit(1);
    }

    // copy bufferText into the resized input string and return it
    strcpy(temp, bufferText);
    return temp;
}

// 8 - invert case
void invertCase(char *eingabetext)
{
    // measure the length
    size_t length = strlen(eingabetext);

    // go through every character in the input text
    for (size_t i = 0; i < length; i++)
    {
        if (eingabetext[i] >= 65 && eingabetext[i] <= 90) // capital letters
        {
            eingabetext[i] += 32;
        }
        else if (eingabetext[i] >= 97 && eingabetext[i] <= 122) // small case letters
        {
            eingabetext[i] -= 32;
        }
    }
}

// 9 - sort
void sort(char *eingabetext)
{
    // measure the length
    size_t eingabeLength = strlen(eingabetext);

    // create bufferText and copy all characters into it
    char bufferText[eingabeLength + 1];
    for (size_t i = 0; i < eingabeLength; i++)
    {
        bufferText[i] = eingabetext[i];
    }
    bufferText[eingabeLength] = '\0';

    // find the smallest character and move it to the next sorted position
    for (size_t i = 0; i < eingabeLength; i++)
    {
        size_t minPosition = i;
        for (size_t j = i + 1; j < eingabeLength; j++)
        {
            if (bufferText[j] < bufferText[minPosition])
            {
                minPosition = j;
            }
        }

        // switch the smallest character into the current position
        char temp = bufferText[i];
        bufferText[i] = bufferText[minPosition];
        bufferText[minPosition] = temp;
    }

    // copy the sorted text back into the input string
    strcpy(eingabetext, bufferText);
}

// 0 - length
char *length(char *eingabetext)
{
    // measure the input length
    size_t eingabeLength = strlen(eingabetext);

    // count how many digits are needed for the length
    size_t digitCount = 1;
    size_t lengthToCheck = eingabeLength;
    while (lengthToCheck >= 10)
    {
        lengthToCheck /= 10;
        digitCount++;
    }

    // create bufferText with space for the digits and original text
    char *bufferText = malloc(digitCount + eingabeLength + 1);
    if (bufferText == NULL)
    {
        perror("malloc failed in length");
        exit(1);
    }

    // add the input length at the beginning of bufferText
    snprintf(bufferText, digitCount + 1, "%zu", eingabeLength);

    // add the original input text after the length
    strcat(bufferText, eingabetext);

    // resize eingabetext to make room for the length digits
    char *temp = realloc(eingabetext, digitCount + eingabeLength + 1);
    if (temp == NULL)
    {
        free(bufferText);
        perror("realloc failed in length");
        exit(1);
    }

    // copy bufferText into the resized input string and return it
    strcpy(temp, bufferText);
    free(bufferText);
    return temp;
}

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

    // Allocate memory with malloc
    char *dynamicInput = malloc(strlen(source) + 1);

    if (dynamicInput == NULL)
    {
        perror("malloc failed");
        return 1;
    }

    // Copy string into the allocated memory
    strcpy(dynamicInput, source);

    // start using the functions
    for (size_t i = 0; i < strlen(Matrikelnummer); i++)
    {
        if (Matrikelnummer[i] == '1')
        {
            print(dynamicInput);
        }
        else if (Matrikelnummer[i] == '2')
        {
            reverse(dynamicInput);
        }
        else if (Matrikelnummer[i] == '3')
        {
            dynamicInput = duplicate(dynamicInput);
        }
        else if (Matrikelnummer[i] == '4')
        {
            if (Matrikelnummer[i + 1] != '\0')
            {
                int jump = Matrikelnummer[i + 1] - '0'; // - '0' to convert into an integer
                increment(jump, dynamicInput);
            }
        }
        else if (Matrikelnummer[i] == '5')
        {
            if (Matrikelnummer[i + 1] != '\0')
            {
                int jump = Matrikelnummer[i + 1] - '0'; // - '0' to convert into an integer
                decrement(jump, dynamicInput);
            }
        }
        else if (Matrikelnummer[i] == '6')
        {
            dynamicInput = countLetters(dynamicInput);
        }
        else if (Matrikelnummer[i] == '7')
        {
            dynamicInput = snakeify(dynamicInput);
        }
        else if (Matrikelnummer[i] == '8')
        {
            invertCase(dynamicInput);
        }
        else if (Matrikelnummer[i] == '9')
        {
            sort(dynamicInput);
        }
        else if (Matrikelnummer[i] == '0')
        {
            dynamicInput = length(dynamicInput);
        }
    }

    // double checking the result in the terminal
    // print(dynamicInput);

    // save the final text in output.txt
    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL)
    {
        perror("could not open output.txt");
        free(dynamicInput);
        return 1;
    }

    if (fputs(dynamicInput, outputFile) == EOF)
    {
        perror("could not write to output.txt");
        fclose(outputFile);
        free(dynamicInput);
        return 1;
    }

    if (fclose(outputFile) != 0)
    {
        perror("could not close output.txt");
        free(dynamicInput);
        return 1;
    }

    // End of main
    free(dynamicInput);
    dynamicInput = NULL;
    return 0;
}