//
// Created by Bill on 2021/10/12.
//

#ifndef VE482P1_GLOBAL_H
#define VE482P1_GLOBAL_H


#include <setjmp.h>

char *inFileName;   // input file name related with redirection
char *outFileName;  // output file name related with redirection
//char *line;         // original input line
//char *conjLine;     // recombinant line
int fdStdIn;        // file descriptor for stdin
int fdStdOut;       // file descriptor for stdout


// signal variables
bool isSubExist;
sigjmp_buf env;

cd_history cd_his;





#endif //VE482P1_GLOBAL_H
