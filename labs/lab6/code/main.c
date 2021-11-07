//
// Created by Bill on 2021/11/7.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>

#define MAX_STR 1024
#define MAX_LENGTH 2048
#define TXT 0
#define CSV 1

// type of function pointer
typedef void(*demofn_t)(char *, char *);


int main(inc argc, char *argv[]) {
    if (argc != 3) {
        printf("Invalid number of operands:\n"
               "require 3, only get %d\n", argc - 1);
        return -1;
    }

    char file_name[MAX_LENGTH];
    strcpy(file_name, argv[1]);
    char *token = strtok(file_name, "_");
    token = strtok(NULL, ".");
    token = strtok(NULL, ".");
    int file_type = strcmp(token, "csv") ? TXT : CSV;

    char dldir[2][20];
    strcpy(dldir[0], "./dl_sort_txt");
    strcpy(dldir[1], "./dl_sort_csv");
    char dlname[2][20];
    strcpy(dlname[0], "./dl_sort_txt.so");
    strcpy(dlname[1], "./dl_sort_csv.so");
    char dlfuncName[2][20];
    strcpy(dlfuncName[0], "sort_txt");
    strcpy(dlfuncName[1], "sort_csv");

    // Go through the directory to find "dl_sort_txt.so", "dl_sort_csv.so".
    int flag = 0;
    DIR *dir;
    struct dirent *dp;
    char *filename = malloc(MAX_STR * sizeof(char));
    dir = opendir(".");
    while ((dp = readdir(dir)) != NULL) {
        memset(filename, '\0', MAX_STR);
        strcpy(filename, dp->d_name);
        if ((strcmp(filename, "dl_sort_txt.so") == 0 && file_type == TXT) ||
            (strcmp(filename, "dl_sort_csv.so") == 0 && file_type == CSV)) {
            flag = 1;
        }
    }
    closedir(dir);
    free(filename);
    if (flag == 0) {
        printf("failed to find \"%s\", exiting...\n", dlname[file_type]);
        exit(-1);
    }


    // Open the dl file
    void *libhandle = dlopen(dlname[file_type], RTLD_NOW);
    if (!libhandle) {
        printf("Error loading dl_sort_txt.so.\n");
        exit(-1);
    }

    // Load the sort_txt function
    demofn_t demofn = dlsym(libhandle, dlfuncName[file_type]);
    if (!demofn) {
        printf("Error loading function sort_txt.\n");
        exit(-1);
    }

    demofn(argv[1], argv[2]);

    dlclose(libhandle);

    return 0;
}



