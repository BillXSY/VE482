#include "util.h"
#include "global.h"

int main() {
    // initialize input
    char *input = (char *) malloc(sizeof(char) * MAX_LINE);
    init_char(input);

    // recombinant input
    char *rcmb_input = (char *) malloc(sizeof(char) * MAX_LINE);



    // initialize jobs name array
    int jobs_cnt = 0;
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

        init_char(rcmb_input);

        prompt("mumsh $ ");

        if (fgets(input, MAX_LINE, stdin) != NULL) {

            // length check
            if (strlen(input) <= 1) {
                continue;
            }

            input[strlen(input) - 1] = '\0';

            strcat(rcmb_input, input);

            argv_info *info = (argv_info *) malloc(sizeof(argv_info));
            char *processed_input = (char *) malloc(sizeof(char) * MAX_LINE);
            char **argv = (char **) malloc(sizeof(char *) * MAX_LINE);

            ProcessInput(input, processed_input, info, argv);

            // unpaired quotation mark or missing redirection files
            WaitForInput(input, rcmb_input, processed_input, info, argv, jobs_cnt, jobs_array, jobs_pid);

            bool isBackground = !strcmp(argv[info->length - 1], "&");
            if (isBackground) {
                jobs_array[jobs_cnt] = (char *) malloc(sizeof(char) * (strlen(rcmb_input) + 8));
                strcpy(jobs_array[jobs_cnt++], rcmb_input);
                argv[info->length - 1] = NULL;
                info->length--;
            }

            isSubExist = true;

            if (!cd(argv) || !jobs(argv, jobs_cnt, jobs_array, jobs_pid)) {
                free(processed_input);
                free(argv);
                free(info);
                continue;
            }

            execute(info, argv, rcmb_input, jobs_cnt, jobs_pid, isBackground);

            if (!isBackground) {
                while (wait(NULL) != -1);
            }
            free(processed_input);
            free(argv);
            free(info);

        } else { // ctrl D
            printf("exit\n");
            freeAll(input, rcmb_input, jobs_cnt, jobs_array, jobs_pid);
            exit(0);
        }

    }
    freeAll(input, rcmb_input, jobs_cnt, jobs_array, jobs_pid);
}
