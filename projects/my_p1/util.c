//
// Created by Bill on 2021/10/11.
//

#include "util.h"
#include "global.h"

void prompt(const char *message) {
    printf("%s", message);
    fflush(stdout);
}

void ProcessInput(char *input, char* rcmb_input, char *processed_input, argv_info *info,char **argv) {

    if (!strcmp(input, "exit")) {
        printf("exit\n");
        exit(0);
    }

    //initialize
    init_char(rcmb_input);
    init_char(processed_input);

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
//    printf("input is: %s\n", input);
    for (int i = 0; i < (int) strlen(input); ++i) {
        printf("input[%d]: %c\n", i, input[i]);
    }
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

int cd(char **args) {
    char buf[256];
    struct passwd *pwd = getpwuid(getuid());

    if (!strcmp(args[0], "cd")) {
        if (args[1] == NULL || !strcmp(args[1], "~")) {
            strcpy(buf, pwd->pw_dir);
            chdir(buf);
        } else if (args[1][0] == '~') {
            strcpy(buf, pwd->pw_dir);
            strncpy(buf + strlen(pwd->pw_dir), args[1] + 1, strlen(args[1]));
            if (chdir(buf) < 0) {
                printf("%s: No such file or directory\n", buf);
                return 0;
            }
        } else if (!strcmp(args[1], "-")) {
            if (!cd_his.valid0) {
                printf("Error: cannot cd to location\n");
                return 0;
            }
            chdir(cd_his.addr[0]);
            printf("%s\n", cd_his.addr[0]);
            strcpy(buf, cd_his.addr[0]);
        } else {
            strcpy(buf, args[1]);
            if (chdir(buf) < 0) {
                printf("%s: No such file or directory\n", buf);
                return 0;
            }
        }
        getcwd(buf, sizeof(buf));
        if (cd_his.valid1) {
            strcpy(cd_his.addr[0], cd_his.addr[1]);
            strcpy(cd_his.addr[1], buf);
        } else if (cd_his.valid0) {
            strcpy(cd_his.addr[1], buf);
            cd_his.valid1 = 1;
        } else {
            strcpy(cd_his.addr[0], buf);
            cd_his.valid0 = 1;
        }
        return 0;
    }
    return 1;
}

int jobs(char **args, int jobs_num, char **jobs_name, int *jobs_pid) {
    if (!strcmp(args[0], "jobs")) {
        for (int j = 0; j < jobs_num; ++j) {
            if (waitpid(jobs_pid[j], NULL, WNOHANG) == 0) {
                printf("[%d] running %s\n", j + 1, jobs_name[j]);
            } else {
                printf("[%d] done %s\n", j + 1, jobs_name[j]);
            }
        }
        return 0;
    }
    return 1;
}