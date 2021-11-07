//
// Created by 71401 on 2021/10/28.
//

#include "ui.h"

List *txt_in(Mode *mode, const char *iFileName) {
    List *list = (List *) malloc(sizeof(List));
    initList(list);

//    printf("reading %s\n", iFileName);

    char *line = malloc(sizeof(char) * MAX_STRING);
    memset(line, 0, MAX_STRING);
    char *tempKey, *tempData;

    FILE *iFile = fopen(iFileName, "r");
    while (fgets(line, MAX_STRING, iFile) != NULL) {
        tempKey = strtok(line, "=");
        tempData = strtok(NULL, "\n");

        void *value;
        if (mode->dataType == DATA_INT) {
            int num = atoi(tempData);
            value = (void *) malloc(sizeof(int));
            *(int *) value = num;
        } else if (mode->dataType == DATA_DUB) {
            double num = atof(tempData);
            value = (void *) malloc(sizeof(double));
            *(double *) value = num;
        } else {
//            printf("str length %lu\n", strlen(tempData));
            value = (void *) malloc(sizeof(char) * (strlen(tempData) + 4));
            memset(value, 0, strlen(tempData) + 4);
            strcpy((char *) value, tempData);
        }
        char *key = (char *) malloc(sizeof(char) * (strlen(tempKey) + 4));
        memset(key, 0, strlen(tempKey) + 4);
        strcpy(key, tempKey);
        insert(list, key, value);
        memset(line, 0, MAX_STRING);
    }
    free(line);
    fclose(iFile);
    return list;
}

void to_txt(Mode *mode, const List *list) {
    char outFileName[100];
    memset(outFileName, 0, 100);
    if (mode->sortType == SORT_INC) {
        strcat(outFileName, "inc_");
    } else if (mode->sortType == SORT_DEC) {
        strcat(outFileName, "dec_");
    } else {
        strcat(outFileName, "rand_");
    }
    if (mode->dataType == DATA_INT) {
        strcat(outFileName, "int.txt");
    } else if (mode->dataType == DATA_DUB) {
        strcat(outFileName, "double.txt");
    } else {
        strcat(outFileName, "char*.txt");
    }
    FILE *oFile = fopen(outFileName, "w");
    printf("writing %s\n", outFileName);
    if (list->length == 0) return;
    Node *nodeIndex;
    for (nodeIndex = list->head; nodeIndex != NULL; nodeIndex = nodeIndex->next) {
        if (mode->dataType == DATA_INT) {
            fprintf(oFile, "%s=%d\n", nodeIndex->key, *(int *) (nodeIndex->value));
        } else if (mode->dataType == DATA_DUB) {
            fprintf(oFile, "%s=%lf\n", nodeIndex->key, *(double *) (nodeIndex->value));
        } else {
            fprintf(oFile, "%s=%s\n", nodeIndex->key, (char *) (nodeIndex->value));
        }
    }
}


