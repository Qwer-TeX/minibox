#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "readline.h"
#include "history.h"

#define BUFFER_SIZE 1024

static struct termios orig_termios;

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_iflag &= ~(IXON);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void clear_line(const char *prompt, char *buffer, int pos) {
    printf("\33[2K\r"); // Clear the current line
    printf("%s%s", prompt, buffer); // Reprint the prompt and buffer content
    printf("\e[%dG", pos + strlen(prompt) + 1); // Move cursor to original position
    fflush(stdout);
}

char *readline(const char *prompt) {
    enable_raw_mode();
    printf("%s", prompt);
    fflush(stdout);

    char *buffer = malloc(BUFFER_SIZE);
    int pos = 0, history_index = get_history_count();
    char c;

    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == '\n') {
            buffer[pos] = '\0';
            printf("\n");
            break;
        } else if (c == 127) { // Handle backspace
            if (pos > 0) {
                pos--;
                printf("\b \b");
                fflush(stdout);
            }
        } else if (c == 27) { // Handle escape sequences (arrow keys and others)
            char seq[3];
            if (read(STDIN_FILENO, &seq[0], 1) == 1 &&
                read(STDIN_FILENO, &seq[1], 1) == 1 &&
                read(STDIN_FILENO, &seq[2], 1) == 1) {
                if (seq[0] == '[' && (seq[1] == 'A' || seq[1] == 'B')) {
                    // Handle arrow keys (Up and Down)
                    if (seq[1] == 'A') { // Up arrow
                        if (history_index > 0) {
                            history_index--;
                            strcpy(buffer, get_history(history_index));
                            pos = strlen(buffer);
                            clear_line(prompt, buffer, pos);
                        }
                    } else if (seq[1] == 'B') { // Down arrow
                        if (history_index < get_history_count() - 1) {
                            history_index++;
                            strcpy(buffer, get_history(history_index));
                            pos = strlen(buffer);
                            clear_line(prompt, buffer, pos);
                        } else if (history_index == get_history_count() - 1) {
                            history_index++;
                            pos = 0;
                            buffer[0] = '\0';
                            clear_line(prompt, buffer, pos);
                        }
                    }
                }
            }
        } else if (c == 4 && pos == 0) { // Handle Ctrl+D at the beginning of the line
            free(buffer);
            disable_raw_mode();
            return NULL;
        } else if (c == 4) { // Handle Ctrl+D to erase characters
            if (pos > 0) {
                pos--;
                printf("\b \b");
                fflush(stdout);
            } else {
                free(buffer);
                disable_raw_mode();
                return NULL;
            }
        } else if (c == 1) { // Handle Ctrl+A (beginning of the line)
            pos = 0;
            printf("\r%s", prompt);
            fflush(stdout);
        } else if (c == 5) { // Handle Ctrl+E (end of the line)
            while (pos < strlen(buffer)) {
                printf("%c", buffer[pos]);
                pos++;
            }
            fflush(stdout);
        } else if (c == 16) { // Handle Ctrl+P (previous history)
            if (history_index > 0) {
                history_index--;
                strcpy(buffer, get_history(history_index));
                pos = strlen(buffer);
                clear_line(prompt, buffer, pos);
            }
        } else if (c == 14) { // Handle Ctrl+N (next history)
            if (history_index < get_history_count() - 1) {
                history_index++;
                strcpy(buffer, get_history(history_index));
                pos = strlen(buffer);
                clear_line(prompt, buffer, pos);
            } else if (history_index == get_history_count() - 1) {
                history_index++;
                pos = 0;
                buffer[0] = '\0';
                clear_line(prompt, buffer, pos);
            }
        } else {
            if (pos < BUFFER_SIZE - 1) {
                buffer[pos++] = c;
                printf("%c", c);
                fflush(stdout);
            }
        }
    }

    if (pos > 0) {
        add_history(buffer);
    }

    disable_raw_mode();
    return buffer;
}

