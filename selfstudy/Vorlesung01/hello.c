#include <stdio.h>

// this is a one line comment

/*  This is a multi line comment
    I can keep typing here and everything is commented out
*/

int addiere(int a, int b)
{
    return a + b;
}

void ausgabe(int x)
{
    printf("%i\n", x);
}

int main(void)
{
    printf("This is my first C programm!\n"); // how to print strings

    // trying the addiere function
    int x = 2;
    int y = 3;
    int c = addiere(x, y);
    printf("if int x =%d, and int y=%d, then x + y gives back: %d\n", x, y, c);

    // trying the ausgabe function
    ausgabe(012); // works and convers 012 from octal to decimal int

    // trying out scanf
    printf("Give me 2 numbers to test my addiere function!\n");
    
    /*  commented out because it is not effective. learned how to scanf properly
    int tmpInput = 0;
    scanf("%d", &tmpInput);
    int userinputA = tmpInput;
    scanf ("%d", &tmpInput);
    int userinputB = tmpInput;
    tmpInput = 0;
    */
    int addA, addB = 0;

    if (scanf("%d %d", &addA, &addB) != 2)
    {
        printf("invalid input! \n");
        return 1;
    }

    printf("You chose the number %d and the number %d to be added! the result is = %d!\n", addA, addB, addiere(addA, addB));

    // closing the main method with return value
    return 0;
}