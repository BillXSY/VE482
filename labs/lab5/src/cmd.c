#include <time.h>

#include "ui.h"


int main(int argc, char const *argv[]) {
    (void) argc;
    srand((unsigned) time(NULL));

    Mode mode;
    getType(&mode, argv[1], argv[2]);

    // read the input file
    List *list = txt_in(&mode, argv[1]);

    // sort
    // checkList(mode, &list);
    sort(&mode, list);
    // checkList(mode, &list);


    // output
    to_txt(&mode, list);

    //free memory
    destList(list);

    return 0;
}
