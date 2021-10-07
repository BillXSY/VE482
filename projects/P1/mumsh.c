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

typedef struct redirect_label {
    int in;
    int out;
    char *inFile;
    char *outFile;
    int inUse;
} redirect_t;

char *inFileName;   // input file name related with redirection
char *outFileName;  // output file name related with redirection
char *line;         // original input line
char *conjLine;     // recombinant line
int fdStdIn;        // file descriptor for stdin
int fdStdOut;       // file descriptor for stdout


void prompt(const char *message) {
    printf("%s", message);
    fflush(stdout);
}


bool ifRedirection(char **argv, int j) {
    if (j == 0) {
        return (strcmp(argv[j], "<") != 0) && (strcmp(argv[j], ">") != 0) && (strcmp(argv[j], ">>") != 0);
    } else {
        return (strcmp(argv[j], "<") != 0) && (strcmp(argv[j], ">") != 0) && (strcmp(argv[j], ">>") != 0) &&
               (strcmp(argv[j - 1], "<") != 0) && (strcmp(argv[j - 1], ">") != 0) && (strcmp(argv[j - 1], ">>") != 0);
    }
}

void redirection_t(redirect_t *r) {
    if (r->inUse == 0) {
        return;
    }
    if (r->in == REDIRECT_IN) {
        int iFD = open(r->inFile, O_CREAT | O_RDONLY, 0666);
        if (iFD > 0) {
            dup2(iFD, 0);
        }
        close(iFD);
    }
    if (r->out == REDIRECT_OUT) {
        int oFD = open(r->outFile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (oFD > 0) {
            dup2(oFD, 1);
        }
        close(oFD);
    } else if (r->out == REDIRECT_APPEND) {
        int oFD = open(r->outFile, O_WRONLY | O_CREAT | O_APPEND, 0666);
        if (oFD > 0) {
            dup2(oFD, 1);
        }
        close(oFD);
    }
}

int main() {
    char *input = (char *) malloc(sizeof(char) * MAX_LINE); // user input
    memset(input, 0, MAX_LINE); // clear input

    redirect_t tmp_re;

    while (1) {
        prompt("mumsh $ ");
        if (fgets(input, MAX_LINE, stdin) != NULL) {
            if (strlen(input) > 1) {
                input[strlen(input) - 1] = '\0';
            } else {
                printf("Too little input\n");
                continue;
            }
            printf("original: %s", input);

            char *real_input = (char *) malloc(sizeof(char) * MAX_LINE);
            memset(real_input, 0, MAX_LINE);

            char **argv = (char **) malloc(sizeof(char *) * MAX_LINE);

            char **real_argv = (char **) malloc(sizeof(char *) * MAX_LINE);

            for (int i = 0; i < MAX_LINE; i++) {
                argv[i] = NULL;
                real_argv[i] = NULL;
            }

            int real_i = 0;

            for (int i = 0; i < (int) strlen(input); ++i) {
                if (input[i] != '<' && input[i] != '>') {
                    real_input[real_i++] = input[i];
                } else if (input[i] == '>' && input[i + 1] == '>') {
                    real_input[real_i++] = ' ';
                    real_input[real_i++] = '>';
                    real_input[real_i++] = '>';
                    real_input[real_i++] = ' ';
                    ++i;
                } else {
                    real_input[real_i++] = ' ';
                    real_input[real_i++] = input[i];
                    real_input[real_i++] = ' ';
                }
            }

            // break down the input with delim = " "
            char *token = strtok(real_input, " ");
            int length = 0;
            while (token != NULL) {
                argv[length++] = token;
                token = strtok(NULL, " ");
            }

            // exit
            if (!strcmp(real_input, "exit")) {
                printf("exit\n");
                exit(0);
            }

            tmp_re.in = 0;
            tmp_re.out = 0;
            for (int j = 0; j < length; ++j) {
                if (!strcmp(argv[j], "<")) {
                    tmp_re.in = REDIRECT_IN;
                    tmp_re.inFile = argv[j + 1];
                }
                if (!strcmp(argv[j], ">")) {
                    tmp_re.out = REDIRECT_OUT;
                    tmp_re.outFile = argv[j + 1];
                }
                if (!strcmp(argv[j], ">>")) {
                    tmp_re.out = REDIRECT_APPEND;
                    tmp_re.outFile = argv[j + 1];
                }
            }

            for (int j = 0, k = 0; j < length; ++j) {
                if (ifRedirection(argv, j)) {
                    real_argv[k++] = argv[j];
                }
            }

            pid_t pid = fork();
            if (pid < 0) {
                printf("ERROR: forking child process failed\n");
                exit(1);
            } else if (pid == 0) {
                redirection_t(&tmp_re);
                if (execvp(real_argv[0], real_argv) < 0) {
                    printf("ERROR: execvp failed\n");
                    exit(1);
                }
            } else {
                wait(NULL);
            }
            free(real_input);
            free(argv);
            free(real_argv);
        }
    }

    free(input);
}
