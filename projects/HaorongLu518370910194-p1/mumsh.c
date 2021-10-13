//
// Created by ancientmodern on 2021/9/17.
//

#include "mumsh.h"

void sigint_handler(int sig_num) {
    if (sig_num) {
        printf("\n");
    }
    if (!isSubExist) {
        siglongjmp(env, 42);
    }
}

int main() {
    // initialize input
    char *input = (char *) malloc(sizeof(char) * MAX_LINE);
    memset(input, 0, MAX_LINE);

    char *rcmb_input = (char *) malloc(sizeof(char) * MAX_LINE);

    // redirection struct
    redirect_t tmp_re;

    int jobs_cnt = 0;

    // jobs name array
    char **jobs_array = (char **) malloc(sizeof(char *) * MAX_LINE);
    for (int i = 0; i < MAX_LINE; i++) {
        jobs_array[i] = NULL;
    }

    // jobs pid
    int *jobs_pid = (int *) malloc(sizeof(int) * MAX_LINE);
    memset(jobs_pid, 0, MAX_LINE);

    // signal handling
    struct sigaction sig;
    sig.sa_handler = sigint_handler;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sig, NULL);


    while (1) {
        sigsetjmp(env, 1);
        isSubExist = false;

        //initialize rcmb_input
        memset(rcmb_input, 0, MAX_LINE);

        prompt("mumsh $ ");
        if (fgets(input, MAX_LINE, stdin) != NULL) { // Not Ctrl D
            // empty input, Only "\n"
            if (strlen(input) <= 1) {
                continue;
            }

            input[strlen(input) - 1] = '\0';
            strcat(rcmb_input, input);

            argv_info *info = (argv_info *) malloc(sizeof(argv_info));

            char *processed_input = (char *) malloc(sizeof(char) * MAX_LINE);

            char **argv = (char **) malloc(sizeof(char *) * MAX_LINE);

            readInput(input, info, processed_input, argv);

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
                    }
                    strcat(rcmb_input, input);
                    strcpy(input, rcmb_input);
                    readInput(input, info, processed_input, argv);
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

            // check if the command is in Background mode
            bool isBackground = !strcmp(argv[info->length - 1], "&");
            if (isBackground) {
                jobs_array[jobs_cnt] = (char *) malloc(sizeof(char) * (strlen(rcmb_input) + 8));
                strcpy(jobs_array[jobs_cnt++], rcmb_input);
                argv[info->length - 1] = NULL;
                info->length--;
            }

            isSubExist = true;

            if (!cd(argv) || !jobs(argv, jobs_cnt, jobs_array, jobs_pid)) {
                free(processed_input);
                free(argv);
                free(info);
                continue;
            }

            int pipe_index = -1;
            bool finish = false;
            pid_t pid;
            int in = 0, fd[2];

            while (!finish) {
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
                        finish = true;
                    }
                }

                if (parse_redirect(&tmp_re, this_argv, count, after_pipe, finish)) {
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
                pid = fork(); // fork a child process
                if (pid < 0) {
                    printf("ERROR: forking child process failed\n");
                    exit(1);
                } else if (pid == 0) { // child process
                    // signal(SIGINT, SIG_DFL);
                    if (in != 0) {
                        dup2(in, 0);
                        close(in);
                    }
                    if (!finish && fd[1] != 1) {
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
                    // close(fd[0]);
                    if (isBackground && finish) {
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
            if (!isBackground) {
                while (wait(NULL) != -1);
            }
            free(processed_input);
            free(argv);
            free(info);
        }
        else { // Ctrl D
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

    free(input);
    free(rcmb_input);
    for (int j = 0; j < jobs_cnt; ++j) {
        free(jobs_array[j]);
    }
    free(jobs_array);
    free(jobs_pid);
}
