#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Generating 1 random int
int zufallZahl() {
    return rand() %10; // using rand to generate a number and Modulu to set an upper limit
}

// Start of the main method
int main (void) {
    srand(time(NULL)); //this generates a random seed for the RNG using the current time
    int x = zufallZahl();
    int y = zufallZahl();
    printf("Here is a random number: %d\n And here is another one: %d\n", x, y);
    return 0;
}