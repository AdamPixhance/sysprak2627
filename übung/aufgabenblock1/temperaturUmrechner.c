#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h> // for the getopt function

// defining my conversion methods on the global scope
// first from all tempretures to Celsius
double CtoC(double sourceTemp)
{
    return sourceTemp;
}
double DetoC(double sourceTemp)
{
    sourceTemp = 100 - sourceTemp * 2.0 / 3.0;
    return sourceTemp;
}
double FtoC(double sourceTemp)
{
    sourceTemp = (sourceTemp - 32) * 5.0 / 9.0;
    return sourceTemp;
}
double KtoC(double sourceTemp)
{
    sourceTemp = sourceTemp - 273.15;
    return sourceTemp;
}
double RatoC(double sourceTemp)
{
    sourceTemp = sourceTemp * 5.0 / 9.0 - 273.15;
    return sourceTemp;
}

// conversion methods from Celsius to all other units
double CtoDe(double tempInC)
{
    return (100 - tempInC) * 1.5;
}
double CtoF(double tempInC)
{
    return tempInC * 1.8 + 32;
}
double CtoK(double tempInC)
{
    return (tempInC + 273.15);
}
double CtoRa(double tempInC)
{
    return tempInC * 1.8 + 491.67;
}

// const strings to reference
const char Celsius[] = "Celsius";
const char Delisle[] = "Delisle";
const char Fahrenheit[] = "Fahrenheit";
const char Kelvin[] = "Kelvin";
const char Rankine[] = "Rankine";

// start of main
int main(int argc, char *argv[])
{
    puts("Temeratur converter started:");

    // preparing arguments for my getopt
    int option;
    const char *from = "x";
    const char *goal = "y";
    // variables for temp calculation
    double sourceTemp = 0;
    double bufferTempInC = 0;
    double resultTemp = 0;

    // starting the while loop with getopt
    // focus on only parsing the input, wihtout handling it yet
    while ((option = getopt(argc, argv, "f:g:t:")) != -1) // f is from, g is goal, t is temprature
    {
        switch (option)
        {
        case 't':
            sourceTemp = (double)strtol(optarg, NULL, 10);
            break;

        case 'f': // dealing with the from input
            // creating a switch case based on what the user chose as a source temp
            switch (optarg[0])
            {
            case 'c':
                from = Celsius;
                break;

            case 'd':
                from = Delisle;
                break;

            case 'f':
                from = Fahrenheit;
                break;

            case 'k':
                from = Kelvin;
                break;

            case 'r':
                from = Rankine;
                break;

            default:
                puts("User did not input a valid value for f(from). Please choose from [c,d,f,k,r] respectivlely for Celsius, delise, farenheit, kelvin and rankine");
                exit(1);
            }
            break;
            // out of dealing with case f

        case 'g':
            switch (optarg[0])
            {
            case 'c':
                goal = Celsius;
                break;

            case 'd':
                goal = Delisle;
                break;

            case 'f':
                goal = Fahrenheit;
                break;

            case 'k':
                goal = Kelvin;
                break;

            case 'r':
                goal = Rankine;
                break;

            default:
                puts("User did not input a valid value for g(goal). Please choose from [c,d,f,k,r] respectivlely for Celsius, delise, farenheit, kelvin and rankine");
                exit(1);
            }
            break;
            // out of dealing with case g

        default:
            puts("User did not input a valid value for f(from), g(goal), and/or t(tempreture). A valid command looks like this:");
            puts("./tempConv -f c -g k -t 36");
            puts("Options for f: and g: are: [c,d,f,k,r] respectivlely for Celsius, delise, farenheit, kelvin and rankine");
            exit(1);
        }
    }
    // The end of our while loop

    // conversion to celsius and storing in bufferTempInC
    if (from == Celsius) {
        bufferTempInC = CtoC(sourceTemp);
    } else if (from == Delisle) {
        bufferTempInC = DetoC(sourceTemp);
    } else if (from == Fahrenheit) {
        bufferTempInC = FtoC(sourceTemp);
    } else if (from == Kelvin) {
        bufferTempInC = KtoC(sourceTemp);
    } else if (from == Rankine) {
        bufferTempInC = RatoC(sourceTemp);
    } else {
        puts("Source temperature unit does not exist in this tool yet. Please make sure your input was correct!");
        exit(1);
    }

    // conversion to goal and saving into resultTemp
    if (goal == Celsius) {
        resultTemp = CtoC(bufferTempInC);
    } else if (goal == Delisle) {
        resultTemp = CtoDe(bufferTempInC);
    } else if (goal == Fahrenheit) {
        resultTemp = CtoF(bufferTempInC);
    } else if (goal == Kelvin) {
        resultTemp = CtoK(bufferTempInC);
    } else if (goal == Rankine) {
        resultTemp = CtoRa(bufferTempInC);
    } else {
        puts("goal temperature unit does not exist in this tool yet. Please make sure your input was correct!");
        exit(1);
    }

    // Output of the converted result
    printf("The conversion from %f %s to %s = %f %s\n", sourceTemp, from, goal, resultTemp, goal);
    // return 0 END
    return 0;
}