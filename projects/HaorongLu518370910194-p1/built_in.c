//
// Created by ancientmodern on 2021/9/21.
//

#include "mumsh.h"

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
