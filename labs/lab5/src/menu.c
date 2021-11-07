#include <time.h>

#include "ui.h"

#define MAX_LENGTH 1024

int main() {
    printf("\nWelcome to VE482 Lab 5 Menu!\n\n");

    while (1) {
        printf("Input format: [Input File] [Sorting Type], or exit\n"
               "Input File: rand_<datatype>.txt\n"
               "Data Type: int, double, char*\n"
               "Sorting Type: rand, inc, dec\n"
               "Example: rand_char*.txt dec\n"
               ">>> ");
        char argv[2][MAX_LENGTH];
        for (int i = 0; i < 2; i++) {
            memset(argv[i], 0, MAX_LENGTH);
        }
        char line[MAX_LENGTH];
        if (fgets(line, MAX_LENGTH, stdin) == NULL) {
            printf("Error: fgets fail.\n");
        }
        if (!strcmp(line, "exit")) {
            printf("Exit the menu!\n");
            break;
        }
        char *token;
        token = strtok(line, " \n");
        strcpy(argv[0], token);
        token = strtok(NULL, " \n");
        strcpy(argv[1], token);

        srand((unsigned) time(NULL));
        Mode mode;
        getType(&mode, argv[0], argv[1]);
        // read the input file
        List *list = txt_in(&mode, argv[0]);
        // sort
        // checkList(mode, &list);
        sort(&mode, list);
        // checkList(mode, &list);
        // output
        to_txt(&mode, list);
        //free memory
        destList(list);
        printf("Mission Completed\n-----------------\n");
        return 0;
    }
}



