//
// Created by 71401 on 2021/10/28.
//

#ifndef HW3_LIST_H
#define HW3_LIST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *key;
    void *value;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    int length;
} List;

void initList(List *list);

void insert(List *list, char *key, void *value);



void destList(List *list);


#endif //HW3_LIST_H
