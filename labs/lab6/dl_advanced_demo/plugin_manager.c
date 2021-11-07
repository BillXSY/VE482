//
// Created by citrate on 2021/10/26.
//
#include <stdio.h>
#include <stdlib.h>
#include "plugin_manager.h"

struct hook_list_t {
    int right;
    hook_t hook;
    struct hook_list_t *next;
};

struct manager_t {
    struct hook_list_t *hook_list;
};

manager *new_manager() {
    manager *m = malloc(sizeof(manager));
    m->hook_list = NULL;
    return m;
}

void free_manager(manager *m) {
    struct hook_list_t *hook_list_node = m->hook_list;
    while (hook_list_node) {
        struct hook_list_t *next = hook_list_node->next;
        free(hook_list_node);
        hook_list_node = next;
    }
    free(m);
}

void register_hook(manager *m, int right, hook_t hook) {
    struct hook_list_t *hook_list_node = malloc(sizeof(struct hook_list_t));
    hook_list_node->right = right;
    hook_list_node->hook = hook;
    hook_list_node->next = m->hook_list;
    m->hook_list = hook_list_node;
}

int apply_hook(manager *m, int *left, int right) {
    struct hook_list_t *hook_list_node = m->hook_list;
    while(hook_list_node){
        if(hook_list_node->right == right){
            int rt_value = hook_list_node->hook(left);
            return rt_value;
        }
        hook_list_node = hook_list_node->next;
    }
    printf("manager: Fail to find suitable plugin to do +%d.\n", right);
    return -1;
}
