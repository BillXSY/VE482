//
// Created by citrate on 2021/10/26.
//

#ifndef DEMO_CODE_PLUGIN_MANAGER_H
#define DEMO_CODE_PLUGIN_MANAGER_H

typedef int(*hook_t)(int *left);

typedef struct manager_t manager;

manager *new_manager();

void free_manager(manager *m);

void register_hook(manager *m, int right, hook_t hook);

int apply_hook(manager *m, int *left, int right);

#endif //DEMO_CODE_PLUGIN_MANAGER_H
