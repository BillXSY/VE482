//
// Created by 71401 on 2021/10/28.
//

#include "list.h"

void initList(List *list) {
    list->head = NULL;
    list->length = 0;
}

void insert(List *list, char *key, void *value) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    newNode->key = key;
    newNode->value = value;
    newNode->next = NULL;

    if (!list->head) {
        list->head = newNode;
    } else {
        Node *currNode = list->head;
        while (currNode->next != NULL) {
            currNode = currNode->next;
        }
        currNode->next = newNode;
    }
    list->length++;
}



void destList(List *list) {
    if (!list->length) {
        return;
    }
    Node *currNode = list->head;
    while (currNode != NULL) {
        Node *victim = currNode;
        Node *nextNode = currNode->next;
        free(victim->key);
        free(victim->value);
        free(victim);
        currNode = nextNode;
    }
}


