//
// Created by Bill on 2021/10/11.
//

#include "util.h"


void prompt(const char *message) {
    printf("%s", message);
    fflush(stdout);
}

void get_input(char *input_processed) {
    char *input = (char *) malloc(sizeof(char) * MAX_LINE);
    initialize(input);

    if (fgets(input, MAX_LINE, stdin) != NULL) {
        if (strlen(input) > 1) {
            input[strlen(input) - 1] = '\0';
        } else {
            printf("Too little input\n");
        }
    }

    int real_i = 0;

    for (int i = 0; i < (int) strlen(input); ++i) {
        if (input[i] != '<' && input[i] != '>') {
            input_processed[real_i++] = input[i];
        } else if (input[i] == '>' && input[i + 1] == '>') {
            input_processed[real_i++] = ' ';
            input_processed[real_i++] = '>';
            input_processed[real_i++] = '>';
            input_processed[real_i++] = ' ';
            ++i;
        } else {
            input_processed[real_i++] = ' ';
            input_processed[real_i++] = input[i];
            input_processed[real_i++] = ' ';
        }
    }

    free(input);

}

int split_input(char *input_processed, char** argv) {
    int length = 0;
    char *token = strtok(input_processed, " ");
    while (token != NULL) {
        argv[length++] = token;
        token = strtok(NULL, " ");
    }
    return length;
}


void initialize(char *input) {
    memset(input, 0, MAX_LINE);
}

void check_input(char *input) {
//    printf("input is: %s\n", input);
    for (int i = 0; i < (int) strlen(input); ++i) {
        printf("input[%d]: %c\n", i, input[i]);
    }
}


void check_argv(char** argv, int argv_length) {
    for (int i = 0; i <  argv_length; ++i) {
        printf("%s\n", argv[i]);
    }
}

