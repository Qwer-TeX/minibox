#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readline.h"
#include "history.h"

void display_history() {
    int count = get_history_count();
    for (int i = 0; i < count; i++) {
        printf("%d: %s\n", i + 1, get_history(i));
    }
}

int main() {
    while (1) {
        char *line = readline("prompt> ");
        if (line == NULL) {
            break;
        }
        if (strcmp(line, "exit") == 0) {
            free(line);
            break;
        } else if (strcmp(line, "history") == 0) {
            display_history();
        } else {
            printf("You entered: %s\n", line);
        }
        free(line);
    }
    return 0;
}

