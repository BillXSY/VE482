#ifndef MUMSH_H
#define MUMSH_H
/* Headers */
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
#include <setjmp.h>

#define MAX_LINE 1030
#define REDIRECT_IN 1
#define REDIRECT_OUT 1
#define REDIRECT_APPEND 2

typedef struct {
    int in;
    int out;
    char *inFile;
    char *outFile;
} redirect_t;

// signal variables
bool isSubExist;
sigjmp_buf env;

typedef struct {
    char addr[2][256];
    int valid0;
    int valid1;
} cd_history;

cd_history cd_his;

typedef struct {
    int sQuote;
    int dQuote;
    int length;
} argv_info;

/* redirection.c */
void redirect_fd(redirect_t *);

bool parse_redirect(redirect_t *r, char **this_argv, int count, bool after_pipe, bool finish);

/* util.c */
bool is_not_redirection(char **, int);

/* built_in.c */
void pwd();

int cd(char **);

int jobs(char **, int, char **, int *);

void readInput(char *, argv_info *, char *, char **);

void prompt(const char *message);

#endif // MUMSH_H
