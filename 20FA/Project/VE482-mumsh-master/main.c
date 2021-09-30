#include <stdio.h>

char *line;
#define MAX_LINE 1030       // maxima size of a command line (1024)

void prompt(const char *message){
    printf("%s", message);
    fflush(stdout);
}

int main() {
    while (1) {
        prompt("mumsh $ ");
        // char str[100]  ;
        fgets(line, MAX_LINE, stdin);
        printf("%s",line);
        // free(line);
    }
    return 0;
}
