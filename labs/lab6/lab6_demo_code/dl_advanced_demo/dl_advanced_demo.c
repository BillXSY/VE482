//
// Created by citrate on 2021/10/26.
//

/*
 * A silly addition calculator that uses plugin to finish the addition.
 * It takes in two integer numbers from command line.
 */

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "plugin_manager.h"

typedef void(*demofn_t)(void);

int main(int argc, char *argv[]) {
    // DISCOVERING
    // I skipped this part in demo.
    // Consider an elegant way to finish discovering, because you need to
    // close plugin files when your program finish execution.

    // REGISTERING
    // It will be much easier to finish registering along with discovering.
    // Because here's just a demo, I register plugins directly.
    typedef void(*regfn_t)(manager *);
    manager *m = new_manager();
    void *handle_3 = dlopen("./dl_3.so", RTLD_NOW);
    void *handle_4 = dlopen("./dl_4.so", RTLD_NOW);
    ((regfn_t) dlsym(handle_3, "init_add_3"))(m);
    ((regfn_t) dlsym(handle_4, "init_add_4"))(m);

    // HOOKING
    // In fact hooks are linked during registration. Here I call the use of
    // hook as hooking.
    int left, right;
    if (argc < 3) {
        left = 0;
        right = 3;
    } else {
        left = atoi(argv[1]);
        right = atoi(argv[2]);
    }
    if (apply_hook(m, &left, right) != 0) {
        printf("main: Fail to finsh %d+%d, reason is showed above.\n", left, right);
        exit(0);
    }
    printf("After add %d to the left value, it becomes %d.\n", right, left);

    dlclose(handle_3);
    dlclose(handle_4);
    return 0;
}
