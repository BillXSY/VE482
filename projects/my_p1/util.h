//
// Created by Bill on 2021/10/11.
//

#ifndef VE482P1_UTIL_H
#define VE482P1_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pwd.h>
#include <stdbool.h>

#define MAX_LINE 1030       // maxima size of a command line (1024)
#define MAX_FILENAME 256    // maxima length of the file name (255)
#define MAX_PIPED 400       // maxima count of pipeline
#define MAX_BGPROC 50       // maxima count of background process

# define REDIRECT_IN 10
# define REDIRECT_OUT 11
# define REDIRECT_APPEND 12

char *inFileName;   // input file name related with redirection
char *outFileName;  // output file name related with redirection
char *line;         // original input line
char *conjLine;     // recombinant line
int fdStdIn;        // file descriptor for stdin
int fdStdOut;       // file descriptor for stdout


typedef struct redirect_label {
    int in;
    int out;
    char *inFile;
    char *outFile;
    int inUse;
} redirect_t;


// print prompt
void prompt(const char *message);

// get user's input
// write into buff
void get_input(char * input_processed);

int split_input(char *input_processed, char** argv);

void initialize(char *input);

void check_input(char *);

void check_argv(char **, int );

#endif //VE482P1_UTIL_H
