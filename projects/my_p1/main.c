#include "util.h"


int main() {


    while (1) {

        prompt("mumsh $ ");

        // initialize processed input
        char *input_processed = (char *) malloc(sizeof(char) * MAX_LINE);
        memset(input_processed, 0, MAX_LINE);

        //get input and process
        get_input(input_processed);

        if (!strcmp(input_processed, "exit")) {
            printf("exit\n");
            exit(0);
        }

        char **argv = (char **) malloc(sizeof(char *) * MAX_LINE);

//        char **real_argv = (char **) malloc(sizeof(char *) * MAX_LINE);

        int argv_length = split_input(input_processed, argv);

        redirect_t tmp_re;
        tmp_re.in = 0;
        tmp_re.out = 0;
        for (int j = 0; j < argv_length; ++j) {
            if (!strcmp(argv[j], "<")) {
                tmp_re.in = REDIRECT_IN;
                tmp_re.inFile = argv[j + 1];
            }
            if (!strcmp(argv[j], ">")) {
                tmp_re.out = REDIRECT_OUT;
                tmp_re.outFile = argv[j + 1];
            }
            if (!strcmp(argv[j], ">>")) {
                tmp_re.out = REDIRECT_APPEND;
                tmp_re.outFile = argv[j + 1];
            }
        }

//        check_argv(argv,argv_length);


        break;
    }
    return 0;
}
