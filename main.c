#include <stdbool.h>
#include "gap_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_help() {
    printf("Available commands:\n");
    printf("1. Append text symbols to the end\n");
    printf("2. Start a new line\n");
    printf("3. Save text to a file\n");
    printf("4. Load text from a file\n");
    printf("5. Print the current text to console\n");
    printf("6. Insert text by line and symbol index\n");
    printf("7. Search for text\n");
    printf("8. Clear console\n");
    printf("0. Exit\n");
}

void handle_input(GapBuffer *gb, const char *command, bool *is_running) {
    char text[256];
    char filename[256];
    int line, index;

    if (strcmp(command, "0") == 0 || strcmp(command, "exit") == 0) {
        gap_buffer_free(gb);
        *is_running = false;
    } else if (strcmp(command, "1") == 0) {
        printf("Enter text to append: ");
        scanf(" %[^\n]", text);
        gap_buffer_insert_text(gb, text);
    } else if (strcmp(command, "2") == 0) {
        printf("New line is started\n");
        gap_buffer_insert_text(gb, "\n");
    } else if (strcmp(command, "3") == 0) {
        printf("Enter the file name for saving: ");
        scanf("%s", filename);
        gap_buffer_save_to_file(gb, filename);
    } else if (strcmp(command, "4") == 0) {
        printf("Enter the file name for loading: ");
        scanf("%s", filename);
        gap_buffer_load_from_file(gb, filename);
    } else if (strcmp(command, "5") == 0) {
        gap_buffer_print(gb);
    } else if (strcmp(command, "6") == 0) {
        printf("Choose line and index: ");
        scanf("%d %d", &line, &index);
        printf("Enter text to insert: ");
        scanf(" %[^\n]", text);
        gap_buffer_insert_at(gb, line, index, text);
    } else if (strcmp(command, "7") == 0) {
        printf("Enter text to search: ");
        scanf(" %[^\n]", text);
        gap_buffer_search(gb, text);
    } else if (strcmp(command, "8") == 0) {
        system("cls");
    } else {
        printf("The command is not implemented yet :(\n");
        print_help();
    }
}

int main(void) {
    GapBuffer *gb = (GapBuffer *)malloc(sizeof(GapBuffer));
    gap_buffer_init(gb);
    bool is_running = true;

    print_help();

    while (is_running) {
        char command[256];
        printf("Choose the command: ");
        scanf("%s", command);
        handle_input(gb, command, &is_running);
    }

    return 0;
}
