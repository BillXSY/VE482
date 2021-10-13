#ifndef VE482P1_GLOBAL_H
#define VE482P1_GLOBAL_H

#include <setjmp.h>

#define MAX_LINE 1024

# define REDIRECT_IN 10
# define REDIRECT_OUT 11
# define REDIRECT_APPEND 12

// signal variables
bool isSubExist;
sigjmp_buf env;

cd_history cd_his;

#endif //VE482P1_GLOBAL_H
