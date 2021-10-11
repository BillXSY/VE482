#include "util.h"

int main() {


    while (1) {
        prompt("mumsh $ ");

        char *input_processed = (char *) malloc(sizeof(char) * MAX_LINE);
        memset(input_processed, 0, MAX_LINE);

        get_input(input_processed);

        if (!strcmp(input_processed, "exit")) {
            printf("exit\n");
            exit(0);
        }

        char **argv = (char **) malloc(sizeof(char *) * MAX_LINE);

//        char **real_argv = (char **) malloc(sizeof(char *) * MAX_LINE);

        int argv_length = split_input(input_processed, argv);

        redirect_t tmp_re;




        check_argv(argv,argv_length);


        break;
    }
    return 0;
}
