#include "util.h"
#include "global.h"

void prompt(const char *message) {
    printf("%s", message);
    fflush(stdout);
}

void ProcessInput(char *input, char *processed_input, argv_info *info, char **argv) {

//    input[strlen(input) - 1] = '\0';
//    check_input(input);


    if (!strcmp(input, "exit")) {
        printf("exit\n");
        exit(0);
    }

    //initialize
    init_char(processed_input);


    for (int i = 0; i < MAX_LINE; i++) {
        argv[i] = NULL;
    }

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
        } else {
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

    int length = split_input(processed_input, argv);

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

//    printf("%d\n", length);
//    check_argv(argv, length);
}

void WaitForInput(char *input, char *rcmb_input, char *processed_input, argv_info *info, char **argv, int jobs_cnt,
                  char **jobs_array, int *jobs_pid) {

    while (info->sQuote != -1 || info->dQuote != -1 || !strcmp(argv[info->length - 1], "<") ||
           !strcmp(argv[info->length - 1], ">") || !strcmp(argv[info->length - 1], "|")) {

        prompt("> ");

        if (fgets(input, MAX_LINE, stdin) != NULL) {
            if (strlen(input) <= 1) {
                continue;
            }
            input[strlen(input) - 1] = '\0';

            if (info->sQuote != -1 || info->dQuote != -1) {
                rcmb_input[strlen(rcmb_input)] = '\n';
            } else if (!strcmp(argv[info->length - 1], "|")) {
                rcmb_input[strlen(rcmb_input)] = ' ';
//                check_input(rcmb_input);
            }
            strcat(rcmb_input, input);
            strcpy(input, rcmb_input);

//            printf("updated input is: ");
//            check_input(input);

            ProcessInput(input, processed_input, info, argv);

        } else {
            // Ctrl D
            printf("exit\n");
            free(input);
            free(rcmb_input);
            for (int j = 0; j < jobs_cnt; ++j) {
                free(jobs_array[j]);
            }
            free(jobs_array);
            free(jobs_pid);
            exit(0);
        }
    }

}

int split_input(char *input_processed, char **argv) {
    int length = 0;
    char *token = strtok(input_processed, " ");
    while (token != NULL) {
        argv[length++] = token;
        token = strtok(NULL, " ");
    }
    return length;
}

void init_char(char *input) {
    memset(input, 0, MAX_LINE);
}

void init_int(int *input) {
    memset(input, 0, MAX_LINE);
}

void check_input(char *input) {
    printf("%s.\n", input);
//    for (int i = 0; i < (int) strlen(input); ++i) {
//        printf("input[%d]: %c\n", i, input[i]);
//    }
}

void check_argv(char **argv, int argv_length) {
    for (int i = 0; i < argv_length; ++i) {
        printf("%s\n", argv[i]);
    }
}

void pwd() {
    char buf[1024];
    getcwd(buf, sizeof(buf));
    printf("%s\n", buf);
    exit(0);
}

int cd(char **argv) {
    char buf[256];
    struct passwd *pwd = getpwuid(getuid());

    if (!strcmp(argv[0], "cd")) {
        if (argv[1] == NULL || !strcmp(argv[1], "~")) {
            strcpy(buf, pwd->pw_dir);
            chdir(buf);
        } else if (argv[1][0] == '~') {
            strcpy(buf, pwd->pw_dir);
            strncpy(buf + strlen(pwd->pw_dir), argv[1] + 1, strlen(argv[1]));
            if (chdir(buf) < 0) {
                printf("%s: No such file or directory\n", buf);
                return 0;
            }
        } else if (!strcmp(argv[1], "-")) {
            if (!cd_his.valid0) {
                printf("Error: cannot cd to location\n");
                return 0;
            }
            chdir(cd_his.address[0]);
            printf("%s\n", cd_his.address[0]);
            strcpy(buf, cd_his.address[0]);
        } else {
            strcpy(buf, argv[1]);
            if (chdir(buf) < 0) {
                printf("%s: No such file or directory\n", buf);
                return 0;
            }
        }
        getcwd(buf, sizeof(buf));
        if (cd_his.valid1) {
            strcpy(cd_his.address[0], cd_his.address[1]);
            strcpy(cd_his.address[1], buf);
        } else if (cd_his.valid0) {
            strcpy(cd_his.address[1], buf);
            cd_his.valid1 = 1;
        } else {
            strcpy(cd_his.address[0], buf);
            cd_his.valid0 = 1;
        }
        return 0;
    }
    return 1;
}

int jobs(char **argv, int jobs_cnt, char **jobs_array, int *jobs_pid) {
    if (!strcmp(argv[0], "jobs")) {
        for (int j = 0; j < jobs_cnt; ++j) {
            if (waitpid(jobs_pid[j], NULL, WNOHANG) == 0) {
                printf("[%d] running %s\n", j + 1, jobs_array[j]);
            } else {
                printf("[%d] done %s\n", j + 1, jobs_array[j]);
            }
        }
        return 0;
    }
    return 1;
}

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

bool is_not_redirection(char **argv, int j) {
    if (j == 0) {
        return (strcmp(argv[j], "<") != 0) && (strcmp(argv[j], ">") != 0) && (strcmp(argv[j], ">>") != 0);
    } else {
        return (strcmp(argv[j], "<") != 0) && (strcmp(argv[j], ">") != 0) && (strcmp(argv[j], ">>") != 0) &&
               (strcmp(argv[j - 1], "<") != 0) && (strcmp(argv[j - 1], ">") != 0) && (strcmp(argv[j - 1], ">>") != 0);
    }
}

void sigint_handler(int sig_num) {
    if (sig_num) {
        printf("\n");
    }
    if (!isSubExist) {
        siglongjmp(env, 42);
    }
}

void freeAll(char *input, char *rcmb_input, int jobs_cnt, char **jobs_array, int *jobs_pid) {
    free(input);
    free(rcmb_input);
    for (int j = 0; j < jobs_cnt; ++j) {
        free(jobs_array[j]);
    }
    free(jobs_array);
    free(jobs_pid);

}

void execute(argv_info *info, char **argv, char *rcmb_input, int jobs_cnt, int *jobs_pid, bool isBackground) {
    // redirection struct
    redirect_t tmp_re;

    int pipe_index = -1;
    bool isFinished = false;
    int in = 0, fd[2];
    pid_t pid;

    while (!isFinished) {
        memset(fd, 0, 2);
        char **this_argv = (char **) malloc(sizeof(char *) * MAX_LINE);
        char **real_argv = (char **) malloc(sizeof(char *) * MAX_LINE);
        for (int i = 0; i < MAX_LINE; i++) {
            this_argv[i] = NULL;
            real_argv[i] = NULL;
        }

        int count = 0;
        bool after_pipe = (pipe_index != -1);
        for (int j = pipe_index + 1; j < info->length; ++j) {
            if (!strcmp(argv[j], "|")) {
                pipe_index = j;
                break;
            } else if (!strcmp(argv[j], "\\|")) {
                this_argv[count++] = "|";
            } else {
                this_argv[count++] = argv[j];
            }
            if (j == info->length - 1) {
                isFinished = true;
            }
        }

        if (parse_redirect(&tmp_re, this_argv, count, after_pipe, isFinished)) {
            free(this_argv);
            free(real_argv);
            break;
        }

        for (int j = 0, k = 0; j < count; ++j) {
            if (!strcmp(this_argv[j], "\\<")) {
                real_argv[k++] = "<";
            } else if (!strcmp(this_argv[j], "\\>")) {
                real_argv[k++] = ">";
            } else if (!strcmp(this_argv[j], "\\>\\>")) {
                real_argv[k++] = ">>";
            } else if (is_not_redirection(this_argv, j)) {
                real_argv[k++] = this_argv[j];
            }
        }

        pipe(fd);
        pid = fork();
        if (pid < 0) {
            printf("ERROR: forking child process failed\n");
            exit(1);
        } else if (pid == 0) {
            if (in != 0) {
                dup2(in, 0);
                close(in);
            }
            if (!isFinished && fd[1] != 1) {
                dup2(fd[1], 1);
                close(fd[1]);
            }
            redirect_fd(&tmp_re);

            if (!strcmp(real_argv[0], "pwd")) {
                pwd();
            } else {
                if (execvp(real_argv[0], real_argv) < 0) {
                    printf("%s: command not found\n", real_argv[0]);
                    exit(1);
                }
            }
        } else { // parent process
            if (isBackground && isFinished) {
                jobs_pid[jobs_cnt - 1] = pid;
                printf("[%d] %s\n", jobs_cnt, rcmb_input);
                waitpid(pid, NULL, WNOHANG);
            }
        }
        close(fd[1]);
        in = fd[0];

        free(this_argv);
        free(real_argv);
    }


}








