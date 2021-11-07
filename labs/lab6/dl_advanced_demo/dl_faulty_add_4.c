//
// Created by citrate on 2021/10/26.
//
#include <stdio.h>
#include "plugin_manager.h"

int add_4(int *left){
    printf("add_4: Faulty add_4 failed to add 4!\n");
    return -1;
}

void init_add_4(manager *m){
    // EXPOSING
    register_hook(m, 4, add_4);
    printf("add_4: Registration Succeed!\n");
}
