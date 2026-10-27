#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 3     // set a constant N for our matrix
#define UPPER 9 // set a constant upper limit

// Function for Generating 1 random int
int zufallZahl()
{
    return rand() % UPPER + 1; // using rand to generate a number and Modulu to set an upper limit
}

// Function for Generating a random number in an array
void zufallZahlArray(int *arrayItem)
{
    *arrayItem = zufallZahl();
}

// Function to print arrays
void printMatrix(int size, int array[size][size])
{ // 2 variables, the size of the array to print, and the array itself
    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            printf("%d   ", array[x][y]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function for taking 2 arrays of the same size and creating a product array
void ArrayMatrixMult(int size, int arrayOne[size][size], int arrayTwo[size][size], int arrayThree[N][N])
{
    for (int a = 0; a < size; a++)
    {
        for (int b = 0; b < size; b++)
        {
            arrayThree[a][b] = 0; // to be sure, set the array item at 0 before the innermost loop
            for (int c = 0; c < size; c++)
            {
                arrayThree[a][b] = arrayThree[a][b] + (arrayOne[a][c] * arrayTwo[c][b]);
            }
        }
    }
}

// Start of the main method
int main(void)
{
    srand(time(NULL)); // this generates a random seed for the RNG using the current time

    // Generating 2 empty arrays
    int arrayOne[N][N] = {0};
    int arrayTwo[N][N] = {0};

    // populating the arrays with the random values
    for (int x = 0; x < N; x++)
    {
        for (int y = 0; y < N; y++)
        {
            zufallZahlArray(&arrayOne[x][y]);
            zufallZahlArray(&arrayTwo[x][y]);
        }
    }

    puts("The random arrays have been generated.");

    // printing out the arrays using the printMatrix custom function
    printf("arrayOne with the dimentsion %d x %d has the following elements:\n", N, N);
    printMatrix(N, arrayOne);
    printf("arrayTwo with the dimentsion %d x %d has the following elements:\n", N, N);
    printMatrix(N, arrayTwo);

    // multiplying both matrixes
    int arrayThree[N][N] = {0}; // creating an arraymatrix set to 0s first
    ArrayMatrixMult(N, arrayOne, arrayTwo, arrayThree);

    // printing out the Product array
    printf("The product array of ArrayOne and ArrayTwo with the dimenstion %d x %d =\n", N, N);
    printMatrix(N, arrayThree);

    /* Testing if the zufallZahl works

    int x = zufallZahl();
    int y = zufallZahl();
    printf("Here is a random number: %d\n And here is another one: %d\n", x, y);

    commented out after test */

    puts("Bye Bye!");
    // End of main method
    return 0;
}