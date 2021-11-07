//
// Created by 71401 on 2021/10/28.
//

#ifndef HW3_UI_H
#define HW3_UI_H

#include "sort.h"

#define MAX_STRING 2048

List *txt_in(Mode *mode, const char *iFileName);

void to_txt(Mode *mode, const List *list);

#endif //HW3_UI_H
