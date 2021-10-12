//
// Created by ancientmodern on 2021/9/17.
//

#include "mumsh.h"

void prompt(const char *message) {
    printf("%s", message);
    fflush(stdout);
}

bool is_not_redirection(char **argv, int j) {
    if (j == 0) {
        return (strcmp(argv[j], "<") != 0) && (strcmp(argv[j], ">") != 0) && (strcmp(argv[j], ">>") != 0);
    } else {
        return (strcmp(argv[j], "<") != 0) && (strcmp(argv[j], ">") != 0) && (strcmp(argv[j], ">>") != 0) &&
               (strcmp(argv[j - 1], "<") != 0) && (strcmp(argv[j - 1], ">") != 0) && (strcmp(argv[j - 1], ">>") != 0);
    }
}

void readInput(char *input, argv_info *info, char *processed_input, char **argv) {

    if (!strcmp(input, "exit")) {
        printf("exit\n");
        exit(0);
    }

    memset(processed_input, 0, MAX_LINE);
    for (int i = 0; i < MAX_LINE; i++) {
        argv[i] = NULL;
    }

    // expand redirection symbols
    int real_i = 0;
    int single_quote = -1;
    int double_quote = -1;
    for (int i = 0; i < (int) strlen(input); ++i) {
        if (input[i] == '\"') {
            if (single_quote == -1) {
                if (double_quote == -1) {
                    double_quote = i;
                } else {
                    double_quote = -1;
                }
            } else {
                processed_input[real_i++] = input[i];
            }
        }
        else if (input[i] == '\'') {
            if (double_quote == -1) {
                if (single_quote == -1) {
                    single_quote = i;
                } else {
                    single_quote = -1;
                }
            } else {
                processed_input[real_i++] = input[i];
            }
        }
        else if (single_quote == -1 && double_quote == -1) {
            if (input[i] != '<' && input[i] != '>') {
                processed_input[real_i++] = input[i];
            } else if (input[i] == '>' && input[i + 1] == '>') {
                processed_input[real_i++] = ' ';
                processed_input[real_i++] = '>';
                processed_input[real_i++] = '>';
                processed_input[real_i++] = ' ';
                ++i;
            } else {
                processed_input[real_i++] = ' ';
                processed_input[real_i++] = input[i];
                processed_input[real_i++] = ' ';
            }
        }
        else {
            if ((input[i] == '|' || input[i] == '<' || input[i] == '>') &&
                ((input[i + 1] == '\"' && input[i - 1] == '\"') ||
                 (input[i + 1] == '\'' && input[i - 1] == '\''))) {
                processed_input[real_i++] = '\\';
                processed_input[real_i++] = input[i];
            } else {
                processed_input[real_i++] = input[i];
            }
        }
    }

    char *p = strtok(processed_input, " ");
    int length = 0;
    while (p != NULL) {
        argv[length++] = p;
        p = strtok(NULL, " ");
    }

    for (int j = 0; j < length; ++j) {
        if ((!strcmp(argv[j], "<") || !strcmp(argv[j], ">") || !strcmp(argv[j], ">>"))) {
            if (j == length - 1) {
                break;
            } else if (!strcmp(argv[j + 1], "<") || !strcmp(argv[j + 1], ">") || !strcmp(argv[j + 1], ">>") ||
                       !strcmp(argv[j + 1], "|")) {
                printf("syntax error near unexpected token `%s\'\n", argv[j + 1]);
                free(processed_input);
                free(argv);
                free(info);
                siglongjmp(env, 42);
            }
        } else if (!strcmp(argv[j], "|")) {
            if (j == length - 1) {
                break;
            } else if (!strcmp(argv[j + 1], "|")) {
                printf("error: missing program\n");
                free(processed_input);
                free(argv);
                free(info);
                siglongjmp(env, 42);
            }
        }
    }

    info->single_quote = single_quote;
    info->double_quote = double_quote;
    info->length = length;
}
