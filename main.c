#include "gap_buffer.h"
#include "string_utils.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*CommandFunc)(GapBuffer *, bool *);

typedef struct {
    const char *name;
    CommandFunc func;
} Command;

void print_help() {
    printf("Available commands:\n");
    printf("help - Print this help message\n");
    printf("exit - Exit the program\n");
    printf("append - Append text to the end of the buffer\n");
    printf("nl - Start a new line\n");
    printf("save - Save the buffer to a file\n");
    printf("load - Load a file into the buffer\n");
    printf("print - Print the contents of the buffer\n");
    printf("insert - Insert text at a specific line and index\n");
    printf("search - Search for text in the buffer\n");
    printf("clear - Clear the console\n");
}

void cmd_help(GapBuffer *gb, bool *is_running) {
    print_help();
}

void unsaved_changes_dialog(const GapBuffer *gb) {
    if (gb->is_saved) {
        return;
    }

    printf("You have unsaved changes. Do you want to save them? (y/n) ");
    char answer;
    scanf(" %c", &answer);
    if (tolower(answer) == 'y') {
        if (gb->filepath != NULL) {
            gap_buffer_save_to_file(gb, gb->filepath);
        } else {
            char filename[256];
            printf("Enter the file name for saving: ");
            scanf("%s", filename);
            gap_buffer_save_to_file(gb, filename);
        }
    }
}

void cmd_exit(GapBuffer *gb, bool *is_running) {
    if (!gb->is_saved) {
        unsaved_changes_dialog(gb);
    }

    *is_running = false;
}

void cmd_append(GapBuffer *gb, bool *is_running) {
    char text[256];
    printf("Enter text to append: ");
    scanf(" %[^\n]", text);
    gap_buffer_append(gb, text);
}

void cmd_newline(GapBuffer *gb, bool *is_running) {
    printf("New line is started\n");
    gap_buffer_append(gb, "\n");
}

void cmd_save(GapBuffer *gb, bool *is_running) {
    char filename[256];
    printf("Enter the file name for saving: ");
    scanf("%s", filename);
    gap_buffer_save_to_file(gb, filename);
}

void cmd_load(GapBuffer *gb, bool *is_running) {
    if (!gb->is_saved) {
        unsaved_changes_dialog(gb);
    }

    char filename[256];
    printf("Enter the file name for loading:");
    scanf("%s", filename);
    gap_buffer_load_from_file(gb, filename);
}

void cmd_print(GapBuffer *gb, bool *is_running) {
    gap_buffer_print(gb);
}

void cmd_insert(GapBuffer *gb, bool *is_running) {
    char text[256];
    int line, index;
    printf("Choose line and column (e.g. 8 2): ");
    fflush(stdout);
    scanf("%d %d", &line, &index);
    printf("Enter text to insert: ");
    scanf(" %[^\n]", text);
    gap_buffer_insert_at(gb, line, index, text);
}

void cmd_search(GapBuffer *gb, bool *is_running) {
    char text[256];
    printf("Enter text to search: ");
    scanf(" %[^\n]", text);
    gap_buffer_search(gb, text);
}

void cmd_clear(GapBuffer *gb, bool *is_running) {
    system("cls");
}

Command commands[] = {
    {"help", cmd_help},
    {"exit", cmd_exit},
    {"append", cmd_append},
    {"nl", cmd_newline},
    {"save", cmd_save},
    {"load", cmd_load},
    {"print", cmd_print},
    {"insert", cmd_insert},
    {"search", cmd_search},
    {"clear", cmd_clear}
};

void handle_input(GapBuffer *gb, const char *command, bool *is_running) {
    int commands_count = sizeof(commands) / sizeof(Command);

    for (int i = 0; i < commands_count; i++) {
        if (strcmp(command, commands[i].name) == 0) {
            commands[i].func(gb, is_running);
            return;
        }
    }

    printf("The command is not implemented yet :(\n");
    print_help();
}

char *get_input_command() {
    char *command = (char *)malloc(256 * sizeof(char));
    printf("[TEDitor]>");
    scanf("%s", command);
    return to_lower(trim_whitespace(command));
}

int main(void) {
    GapBuffer *gb = (GapBuffer *)malloc(sizeof(GapBuffer));
    gap_buffer_init(gb);
    bool is_running = true;

    print_help();

    while (is_running) {
        const char *command = get_input_command();
        handle_input(gb, command, &is_running);
    }

    // gap_buffer_append(gb, "\n");
    // gap_buffer_append(gb, "line2");
    // gap_buffer_insert_at(gb, 0, 0, "line1");
    // gap_buffer_append(gb, "\n");
    // gap_buffer_insert_at(gb, 2, 0, "line3");
    // gap_buffer_print(gb);

    gap_buffer_free(gb);
    return 0;
}