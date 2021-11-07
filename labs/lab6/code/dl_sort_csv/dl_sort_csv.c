#include <time.h>

#include "ui.h"


int sort_csv(int argc, char const *argv[]) {
    return 0;
    srand((unsigned) time(NULL));
    if (argc != 3) {
        printf("Missing arguments!\n");
        return -1;
    }

    Mode mode;
    getType(&mode, (char **) argv);

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
