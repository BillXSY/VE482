#include <time.h>

#include "ui.h"


void sort_txt(char* argv1, char *argv2) {
    srand((unsigned) time(NULL));

/*    if (argc != 3) {
        printf("Missing arguments!\n");
        return -1;
    }*/


    Mode mode;
    getType(&mode, argv1, argv2);


    // read the input file
    List *list = txt_in(&mode, argv1);

    // sort
    // checkList(mode, &list);
    sort(&mode, list);
    // checkList(mode, &list);


    // output
    to_txt(&mode, list);

    //free memory
    destList(list);
}
