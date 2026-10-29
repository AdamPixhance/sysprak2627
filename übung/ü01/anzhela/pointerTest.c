#include <stdio.h>

int main(void) {
    int a = 99;
    int *pointerA = &a;
    printf ("The value of *pointerA= %d\n", *pointerA);

    int x = *pointerA;
    printf ("The value of int x= %d\n", x);
    return 0;
}