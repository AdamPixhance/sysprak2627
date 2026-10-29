#include <stdio.h>
#include <float.h>
#include <stdlib.h>

int main (int argc, char **argv) { //convention to write a main method this way
    printf("%d\n", argc);
    for (size_t i = 0; i <argc; i++) {
        printf("%s\n", argv[i]);
    }
    printf("hello\n");

    // filename verarbeit
    char * path = argv[1];

    // file öffnen
    FILE *file = fopen("findMaxOfFloats.txt", "r"); //fopen opens a file
    if (file == NULL) {
        perror("fopen did not work");
        return -1;
    }
    printf("file gefunden\n");

    //zahlen abspeichern
    char buf[256];
    float * numbers = NULL; //wc -l gives back the number of lines
    size_t index = 0; //size_t is an alias for the longest int on the system. for example long int, or long long int

    while(1) {
        char *tmp = fgets(buf, 256, file); //fgets reads a file
        if (tmp == NULL) {
            perror("alloc");
            fclose(file);
            return -1;
        }
        float * flt_tmp = realloc(numbers, (index+1)*sizeof(float));
        numbers = flt_tmp;
        sscanf(buf, "%f\n", &numbers[index]); //"%f\n" the \n makes the sscanf ignore white spaces
        index++;
    }

    //max finden
    float max = -FLT_MAX;
    for (size_t i = 0; i<index; i++) {
        if (numbers[i] > max) {
            max = numbers[i];
        }
    }

    //ergebnis
    printf("%.2f\n", max);

    free(numbers);
    fclose(file);
    return 0;
}