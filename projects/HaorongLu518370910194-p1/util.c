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
    int sQuote = -1;
    int dQuote = -1;
    for (int i = 0; i < (int) strlen(input); ++i) {
        // doule quote
        if (input[i] == '\"') {
            if (sQuote == -1) {
                if (dQuote == -1) {
                    dQuote = i;
                } else {
                    dQuote = -1;
                }
            } else {
                processed_input[real_i++] = input[i];
            }
        }
        // single quote
        else if (input[i] == '\'') {
            if (dQuote == -1) {
                if (sQuote == -1) {
                    sQuote = i;
                } else {
                    sQuote = -1;
                }
            } else {
                processed_input[real_i++] = input[i];
            }
        }
        // hasn't find a quote mark yet
        else if (sQuote == -1 && dQuote == -1) {
            if (input[i] != '<' && input[i] != '>') {
                processed_input[real_i++] = input[i];
            }
            // add space before and after >>
            else if (input[i] == '>' && input[i + 1] == '>') {
                processed_input[real_i++] = ' ';
                processed_input[real_i++] = '>';
                processed_input[real_i++] = '>';
                processed_input[real_i++] = ' ';
                ++i;
            }
            // add space before and after ordinary input
            else {
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

    info->sQuote = sQuote;
    info->dQuote = dQuote;
    info->length = length;
}
