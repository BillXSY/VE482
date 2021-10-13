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

typedef struct redirect_label {
    int in;
    int out;
    char *inFile;
    char *outFile;
    int inUse;
} redirect_t;

typedef struct {
    char address[2][256];
    int valid0;
    int valid1;
} cd_history;

typedef struct {
    int sQuote;
    int dQuote;
    int length;
} argv_info;


// print prompt
void prompt(const char *);

// get user's input, write into processed_input
// process argv info and argv
void ProcessInput(char *input, char *processed_input, argv_info *info, char **argv);

void WaitForInput(char *input, char *rcmb_input, char *processed_input, argv_info *info, char **argv, int jobs_cnt,
                  char **jobs_array, int *jobs_pid);

int split_input(char *input_processed, char **argv);

void init_char(char *);

void init_int(int *);

void check_input(char *);

void check_argv(char **, int);

void pwd();

int cd(char **);

int jobs(char **, int, char **, int *);

bool parse_redirect(redirect_t *r, char **this_argv, int count, bool after_pipe, bool finish);

void redirect_fd(redirect_t *r);

bool is_not_redirection(char **argv, int j);

void sigint_handler(int sig_num);

void freeAll(char *input, char *rcmb_input, int jobs_cnt, char **obs_array, int *jobs_pid);

void execute(argv_info *info, char **argv, char *rcmb_input, int jobs_cnt, int *jobs_pid, bool isBackground);

#endif //VE482P1_UTIL_H
