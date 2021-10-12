//
// Created by ancientmodern on 2021/9/17.
//

#include "mumsh.h"

bool parse_redirect(redirect_t *r, char **this_argv, int count, bool after_pipe, bool finish) {
    r->in = 0;
    r->out = 0;

    for (int j = 0; j < count; ++j) {
        if (!strcmp(this_argv[j], "<")) {
            if (r->in == REDIRECT_IN || after_pipe) {
                printf("error: duplicated input redirection\n");
                return true;
            }
            r->in = REDIRECT_IN;
            r->inFile = this_argv[j + 1];
        }
        if (!strcmp(this_argv[j], ">")) {
            if (r->out == REDIRECT_OUT || r->out == REDIRECT_APPEND || !finish) {
                printf("error: duplicated output redirection\n");
                return true;
            }
            r->out = REDIRECT_OUT;
            r->outFile = this_argv[j + 1];
        }
        if (!strcmp(this_argv[j], ">>")) {
            if (r->out == REDIRECT_OUT || r->out == REDIRECT_APPEND || !finish) {
                printf("error: duplicated output redirection\n");
                return true;
            }
            r->out = REDIRECT_APPEND;
            r->outFile = this_argv[j + 1];
        }
    }
    return false;
}

void redirect_fd(redirect_t *r) {
    if (r->in == REDIRECT_IN) {
        int iFD = open(r->inFile, O_RDONLY, 0666);
        if (iFD > 0) {
            dup2(iFD, 0);
        } else if (errno == ENOENT) {
            printf("%s: No such file or directory\n", r->inFile);
            exit(0);
        }
        close(iFD);
    }
    if (r->out == REDIRECT_OUT) {
        int oFD = open(r->outFile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (oFD > 0) {
            dup2(oFD, 1);
        } else if (errno == EPERM || errno == EROFS) {
            printf("%s: Permission denied\n", r->outFile);
            exit(0);
        }
        close(oFD);
    } else if (r->out == REDIRECT_APPEND) {
        int oFD = open(r->outFile, O_WRONLY | O_CREAT | O_APPEND, 0666);
        if (oFD > 0) {
            dup2(oFD, 1);
        } else if (errno == EPERM || errno == EROFS) {
            printf("%s: Permission denied\n", r->outFile);
            exit(0);
        }
        close(oFD);
    }
}
