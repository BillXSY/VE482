//
// Created by citrate on 2021/10/26.
//
#include <stdio.h>
#include "plugin_manager.h"

int add_3(int *left){
    *left += 3;
    return 0;
}

void init_add_3(manager *m){
    // EXPOSING
    register_hook(m, 3, add_3);
    printf("add_3: Registration Succeed!\n");
}
