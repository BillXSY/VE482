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


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Invalid number of operands:\n"
               "require 3, only get %d\n", argc - 1);
        return -1;
    }

    char fileName[MAX_LENGTH];
    strcpy(fileName, argv[1]);
    char *token = strtok(fileName, "_");
    token = strtok(NULL, ".");
    token = strtok(NULL, ".");
    int file_type = strcmp(token, "csv") ? TXT : CSV;
//    printf("file_type is %d\n", file_type);

    char dlDir[2][20];
    strcpy(dlDir[0], "./dl_sort_txt");
    strcpy(dlDir[1], "./dl_sort_csv");

    char dlName[2][20];
    strcpy(dlName[0], "./dl_sort_txt.so");
    strcpy(dlName[1], "./dl_sort_csv.so");

    char dlFuncName[2][20];
    strcpy(dlFuncName[0], "sort_txt");
    strcpy(dlFuncName[1], "sort_csv");

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
        printf("failed to find \"%s\", exiting...\n", dlName[file_type]);
        exit(-1);
    }


    // Open the dl file
    void *libhandle = dlopen(dlName[file_type], RTLD_NOW);
    if (!libhandle) {
        printf("Error loading dl_sort_txt.so.\n");
        exit(-1);
    }

    // Load the sort_txt function
//    printf("%s\n", dlfuncName[file_type]);

    demofn_t demofn = (void (*)(char *, char *)) dlsym(libhandle, dlFuncName[file_type]);
    if (!demofn) {
        printf("Error loading function sort_txt.\n");
        exit(-1);
    }

    demofn(argv[1], argv[2]);

    dlclose(libhandle);

    return 0;
}



