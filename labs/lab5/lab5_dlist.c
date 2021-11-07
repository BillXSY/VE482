//
// Created by 71401 on 2021/10/28.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lab5_dlist.h"

typedef struct Node {
    char *key;
    dlistValue value;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    dlistValueType type;
    int length;
} List;

dlist createDlist(dlistValueType type) {
    if (type == DLIST_UNKOWN) {
        return NULL;
    }
    dlist list = (List *) malloc(sizeof(List));
    ((List *) list)->type = type;
    ((List *) list)->head = NULL;
    ((List *) list)->length = 0;
    return list;
}

int dlistIsEmpty(dlist_const this) {
    return ((List *) this)->length == 0;
}

void dlistAppend(dlist this, const char *key, dlistValue value) {
    Node *node = (Node *) malloc(sizeof(Node));
    if (((List *) this)->type == DLIST_STR) {
        node->value.strValue = (char *) malloc(sizeof(char) * (strlen(value.strValue) + 4));
        strcpy(node->value.strValue, value.strValue);
    } else {
        node->value = value;
    }
    node->key = (char *) malloc(sizeof(char) * (strlen(key) + 4));
    strcpy(node->key, key);
    node->next = NULL;
    if (dlistIsEmpty(this)) {
        ((List *) this)->head = node;
    } else {
        Node *indexNode = ((List *) this)->head;
        while (indexNode->next != NULL) {
            indexNode = indexNode->next;
        }
        indexNode->next = node;
    }
    ((List *) this)->length++;

}

static inline bool rand_data(const void *lhs, const void *rhs) {
    (void) lhs;
    (void) rhs;
    return (rand() % 2 - 1) >= 0;
}

static inline bool inc_int(const void *lhs, const void *rhs) {
    return ((Node *) lhs)->value.intValue >= ((Node *) rhs)->value.intValue;
}

static inline bool dec_int(const void *lhs, const void *rhs) {
    return ((Node *) rhs)->value.intValue >= ((Node *) lhs)->value.intValue;
}

static inline bool inc_double(const void *lhs, const void *rhs) {
    return ((Node *) lhs)->value.doubleValue >= ((Node *) rhs)->value.doubleValue;
}

static inline bool dec_double(const void *lhs, const void *rhs) {
    return ((Node *) rhs)->value.doubleValue >= ((Node *) lhs)->value.doubleValue;
}

static inline bool inc_str(const void *lhs, const void *rhs) {
    return strcmp(((Node *) lhs)->value.strValue, (((Node *) rhs)->value.strValue)) >= 0;
}

static inline bool dec_str(const void *lhs, const void *rhs) {
    return strcmp(((Node *) rhs)->value.strValue, (((Node *) lhs)->value.strValue)) >= 0;
}

bool (*cmp[3][3])(const void *, const void *) = {
        {rand_data, inc_int,    dec_int},
        {rand_data, inc_str,    dec_str},
        {rand_data, inc_double, dec_double}
};

Node *merge(Node *head1, Node *head2, dlistValueType type, dlistSortMethod method) {
    Node *dummyHead = (Node *) malloc(sizeof(Node));
    Node *temp = dummyHead;
    Node *temp1 = head1;
    Node *temp2 = head2;
    while (temp1 != NULL && temp2 != NULL) {
        if (cmp[type - 1][method - 1](temp2, temp1)) {
            temp->next = temp1;
            temp1 = temp1->next;
        } else {
            temp->next = temp2;
            temp2 = temp2->next;
        }
        temp = temp->next;
    }
    if (temp1 != NULL) {
        temp->next = temp1;
    } else if (temp2 != NULL) {
        temp->next = temp2;
    }
    Node *ans = dummyHead->next;
    free(dummyHead);
    return ans;
}

void dlistSort(dlist_const this, dlist listDst, dlistSortMethod method) {
    List *list = (List *) this;
    if (list->length == 0) {
        return;
    }
    Node *head = list->head;
    Node *dummyHead = (Node *) malloc(sizeof(Node));
    dummyHead->next = head;
    for (int subLength = 1; subLength < list->length; subLength <<= 1) {
        Node *prev = dummyHead, *curr = dummyHead->next;
        while (curr != NULL) {
            Node *head1 = curr;
            for (int i = 1; i < subLength && curr->next != NULL; i++) {
                curr = curr->next;
            }
            Node *head2 = curr->next;
            curr->next = NULL;
            curr = head2;
            for (int i = 1; i < subLength && curr != NULL && curr->next != NULL;
                 i++) {
                curr = curr->next;
            }
            Node *next = NULL;
            if (curr != NULL) {
                next = curr->next;
                curr->next = NULL;
            }
            Node *merged = merge(head1, head2, list->type, method);
            prev->next = merged;
            while (prev->next != NULL) {
                prev = prev->next;
            }
            curr = next;
        }
    }

    list->head = dummyHead->next;

    if (!((List *) listDst)->length) {
        for (Node *nodeIndex = ((List *) listDst)->head; nodeIndex != NULL;) {
            Node *nodeOld = nodeIndex;
            nodeIndex = nodeIndex->next;
            if (((List *) this)->type == DLIST_STR) {
                free(nodeOld->value.strValue);
            }
            free(nodeOld->key);
            free(nodeOld);
        }
        ((List *) listDst)->head = NULL;
        ((List *) listDst)->length = 0;
    }

    for (Node *p = dummyHead->next; p != NULL; p = p->next) {
        dlistAppend(listDst, p->key, p->value);
    }
    free(dummyHead);
}

void dlistPrint(dlist_const this) {
    if (((List *) this)->length == 0) {
        return;
    }
    Node *nodeIndex;
    for (nodeIndex = ((List *) this)->head; nodeIndex != NULL; nodeIndex = nodeIndex->next) {
        if (((List *) this)->type == DLIST_INT) {
            printf("%s=%d\n", nodeIndex->key, (nodeIndex->value.intValue));
        } else if (((List *) this)->type == DLIST_DOUBLE) {
            printf("%s=%lf\n", nodeIndex->key, (nodeIndex->value.doubleValue));
        } else {
            printf("%s=%s\n", nodeIndex->key, (nodeIndex->value.strValue));
        }
    }
}

void dlistFree(dlist this) {
    if (!((List *) this)->length) {
        return;
    }
    for (Node *nodeIndex = ((List *) this)->head; nodeIndex != NULL;) {
        Node *nodeOld = nodeIndex;
        nodeIndex = nodeIndex->next;
        if (((List *) this)->type == DLIST_STR) {
            free(nodeOld->value.strValue);
        }
        free(nodeOld->key);
        free(nodeOld);
    }
    free(this);
}

