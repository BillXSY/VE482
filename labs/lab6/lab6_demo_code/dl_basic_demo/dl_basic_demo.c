//
// Created by citrate on 2021/10/26.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>

typedef void(*demofn_t)(void);

int main() {
    // Go through the directory to find "dl_helloworld.so".
    int flag = 0;
    DIR *dir;
    struct dirent *dp;
    char *filename = malloc(1024 * sizeof(char));
    dir = opendir(".");
    printf("Under current directory, I find:\n");
    while ((dp = readdir(dir)) != NULL) {
        memset(filename, '\0', 1024);
        strcpy(filename, dp->d_name);
        printf("%s\n", filename);
        if (strcmp(filename, "dl_helloworld.so") == 0) {
            flag = 1;
        }
    }
    closedir(dir);
    free(filename);
    if (flag == 0) {
        printf("I failed to find \"dl_helloworld.so\", exiting...\n");
        exit(-1);
    }

    // Open the dl file
    // you have to give "./" to indicate current directory
    void *libhandle = dlopen("./dl_helloworld.so", RTLD_NOW);
    if (!libhandle) {
        printf("Error loading dl_helloworld.so.\n");
        exit(-1);
    }
    // Load the helloworld function
    demofn_t demofn = dlsym(libhandle, "helloworld");
    if (!demofn) {
        printf("Error loading function helloworld.\n");
        exit(-1);
    }

    // What will happen if you close the lib here?
    // dlclose(libhandle);

    printf("Successfully loaded function \"helloworld\", executing it...\n");
    demofn();

    dlclose(libhandle);
    return 0;
}
