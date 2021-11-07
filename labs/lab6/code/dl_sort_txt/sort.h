//
// Created by 71401 on 2021/10/28.
//

#ifndef HW3_SORT_H
#define HW3_SORT_H

#include "list.h"

#define SORT_RAND  0
#define SORT_INC   1
#define SORT_DEC   2

#define DATA_INT   0
#define DATA_DUB   1
#define DATA_CHAR  2

typedef struct mode {
    int dataType;
    int sortType;
} Mode;


void getType(Mode *mode, char *argv1, char* argv2);

void sort(Mode *mode, List *list);

void checkList(Mode *mode, const List *list);


#endif //HW3_SORT_H


