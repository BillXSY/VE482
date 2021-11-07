//
// Created by 71401 on 2021/10/28.
//

#include "sort.h"

void getType(Mode *mode, char *argv1,  char *argv2) {
    if (!strcmp(argv1, "rand_int.txt")) {
        mode->dataType = DATA_INT;
    } else if (!strcmp(argv1, "rand_double.txt")) {
        mode->dataType = DATA_DUB;
    } else if (!strcmp(argv1, "rand_char*.txt")) {
        mode->dataType = DATA_CHAR;
    }
    if (!strcmp(argv2, "rand")) {
        mode->sortType = SORT_RAND;
    } else if (!strcmp(argv2, "inc")) {
        mode->sortType = SORT_INC;
    } else if (!strcmp(argv2, "dec")) {
        mode->sortType = SORT_DEC;
    }
}

static inline bool rand_data(const void *lhs, const void *rhs) {
    (void) lhs;
    (void) rhs;
    return (rand() % 2 - 1) >= 0;
}

static inline bool inc_int(const void *lhs, const void *rhs) {
    return *((int *) (((Node *) lhs)->value)) >= *((int *) (((Node *) rhs)->value));
}

static inline bool dec_int(const void *lhs, const void *rhs) {
    return *(int *) (((Node *) rhs)->value) >= *(int *) (((Node *) lhs)->value);
}

static inline bool inc_double(const void *lhs, const void *rhs) {
    return *(double *) (((Node *) lhs)->value) >= *(double *) (((Node *) rhs)->value);
}

static inline bool dec_double(const void *lhs, const void *rhs) {
    return *(double *) (((Node *) rhs)->value) >= *(double *) (((Node *) lhs)->value);
}

static inline bool inc_char(const void *lhs, const void *rhs) {
    return strcmp((char *) (((Node *) lhs)->value), (char *) (((Node *) rhs)->value)) >= 0;
}

static inline bool dec_char(const void *lhs, const void *rhs) {
    return strcmp((char *) (((Node *) rhs)->value), (char *) (((Node *) lhs)->value)) >= 0;
}

bool (*cmp[3][3])(const void *, const void *) = {
        {rand_data, inc_int,    dec_int},
        {rand_data, inc_double, dec_double},
        {rand_data, inc_char,   dec_char}
};

Node *merge(Node *head1, Node *head2, Mode *mode) {
    Node *dummyHead = (Node *) malloc(sizeof(Node));
    dummyHead->value = 0;
    Node *temp = dummyHead, *temp1 = head1, *temp2 = head2;
    while (temp1 != NULL && temp2 != NULL) {
        if (cmp[mode->dataType][mode->sortType](temp2, temp1)) {
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

void sort(Mode *mode, List *list) {
    printf("sorting elements\n");
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
            Node *merged = merge(head1, head2, mode);
            prev->next = merged;
            while (prev->next != NULL) {
                prev = prev->next;
            }
            curr = next;
        }
    }
    list->head = dummyHead->next;
    free(dummyHead);
}

void checkList(Mode *mode, const List *list) {
    Node *nodeIndex;
    for (nodeIndex = list->head; nodeIndex != NULL; nodeIndex = nodeIndex->next) {
        if (mode->dataType == DATA_INT) {
            printf("%s=%d\n", nodeIndex->key, *(int *) (nodeIndex->value));
        } else if (mode->dataType == DATA_DUB) {
            printf("%s=%lf\n", nodeIndex->key, *(double *) (nodeIndex->value));
        } else {
            printf("%s=%s\n", nodeIndex->key, (char *) (nodeIndex->value));
        }
    }
}


