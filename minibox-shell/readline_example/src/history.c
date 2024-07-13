#include <stdlib.h>
#include <string.h>
#include "history.h"

#define HISTORY_SIZE 100

static char *history[HISTORY_SIZE];
static int history_count = 0;

void add_history(const char *line) {
    if (history_count < HISTORY_SIZE) {
        history[history_count++] = strdup(line);
    } else {
        free(history[0]);
        memmove(history, history + 1, (HISTORY_SIZE - 1) * sizeof(char *));
        history[HISTORY_SIZE - 1] = strdup(line);
    }
}

char *get_history(int index) {
    if (index < 0 || index >= history_count) {
        return NULL;
    }
    return history[index];
}

int get_history_count(void) {
    return history_count;
}
