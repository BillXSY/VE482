#include "util.h"
#include "global.h"

void sigint_handler(int sig_num) {
    if (sig_num) {
        printf("\n");
    }
    if (!is_running_sub) {
        siglongjmp(env, 42);
    }
}

int main() {
    // initialize input
    char *input = (char *) malloc(sizeof(char) * MAX_LINE);
    init_char(input);

    // initialize recombinant input
    char *rcmb_input = (char *) malloc(sizeof(char) * MAX_LINE);

    // redirection struct
    redirect_t tmp_re;

    int jobs_cnt = 0;

    // initialize jobs name array
    char **jobs_array = (char **) malloc(sizeof(char *) * MAX_LINE);
    for (int i = 0; i < MAX_LINE; i++) {
        jobs_array[i] = NULL;
    }

    // initialize jobs pid
    int *jobs_pid = (int *) malloc(sizeof(int) * MAX_LINE);
    init_int(jobs_pid);

    // signal handling
    struct sigaction sig;
    sig.sa_handler = sigint_handler;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sig, NULL);

    while (1) {
        sigsetjmp(env, 1);
        isSubExist = false;



        prompt("mumsh $ ");

        if (fgets(input, MAX_LINE, stdin) != NULL) {

            // length check
            if (strlen(input) > 1) {
                input[strlen(input) - 1] = '\0';
            } else {// empty input, Only "\n"
                printf("Too little input\n");
            }
            input[strlen(input) - 1] = '\0';

            strcat(rcmb_input, input);

            argv_info *info = (argv_info *) malloc(sizeof(argv_info));

            char *processed_input = (char *) malloc(sizeof(char) * MAX_LINE);

            char **argv = (char **) malloc(sizeof(char *) * MAX_LINE);

            ProcessInput(input, rcmb_input, processed_input, info, argv);




        }
        else { // ctrl D
            printf("exit\n");
            free(input);
            free(rcmb_input);
            for (int j = 0; j < jobs_cnt; ++j) {
                free(jobs_array[j]);
            }
            free(jobs_array);
            free(jobs_pid);
            exit(0);
        }

        break;
    }

    free(input);
    free(rcmb_input);
    for (int j = 0; j < jobs_cnt; ++j) {
        free(jobs_array[j]);
    }
    free(jobs_array);
    free(jobs_pid);
    return 0;
}
