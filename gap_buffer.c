#include "gap_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER_SIZE 100
#define DEFAULT_WINDOW_SIZE 10

void gap_buffer_init(GapBuffer *gb) { //done
    gb->buffer = (char *)malloc(INITIAL_BUFFER_SIZE * sizeof(char));
    gb->length = INITIAL_BUFFER_SIZE;
    gb->gap.from = 0;
    gb->gap.to = INITIAL_BUFFER_SIZE;
}

void gap_buffer_free(GapBuffer *gb) {//done
    free(gb->buffer);
    free(gb);
}

int gap_get_length(const GapWindow *gap) {//done
    return gap->to - gap->from;
}

void gap_buffer_expand(GapBuffer *gb, int additional_length) {//done
    const int new_length = gb->length + additional_length + DEFAULT_WINDOW_SIZE;
    char *new_buffer = (char *)malloc(new_length * sizeof(char));

    memcpy(new_buffer, gb->buffer, gb->gap.from);

    const int length_after_gap = gb->length - gb->gap.to;
    memcpy(
        new_buffer + new_length - length_after_gap,
        gb->buffer + gb->gap.to,
        length_after_gap);

    gb->gap.to = new_length - length_after_gap;
    gb->length = new_length;

    free(gb->buffer);
    gb->buffer = new_buffer;
}

void gap_buffer_insert_text(GapBuffer *gb, const char *text) {// done
    const int text_length = strlen(text);
    const int gap_length = gap_get_length(&gb->gap);

    if (text_length > gap_length)
        gap_buffer_expand(gb, text_length - gap_length);

    memcpy(gb->buffer + gb->gap.from, text, text_length);
    gb->gap.from += text_length;
}

void move_gap(GapBuffer *gb, int position) { //done
    if (position < gb->gap.from) {
        int move_length = gb->gap.from - position;

        memmove(
            gb->buffer + gb->gap.to - move_length,
            gb->buffer + position,
            move_length);

        gb->gap.to -= move_length;
        gb->gap.from -= move_length;

    } else if (position > gb->gap.from) {
        int move_length = position - gb->gap.from;

        memmove(
            gb->buffer + gb->gap.from,
            gb->buffer + gb->gap.to,
            move_length);

        gb->gap.to += move_length;
        gb->gap.from += move_length;
    }
}

void gap_buffer_delete_text(GapBuffer *gb, int position, int length) {
    move_gap(gb, position);
    gb->gap.to += length;
}

void gap_buffer_print(const GapBuffer *gb) { //done
    for (int i = 0; i < gb->length; i++) {
        if (i == gb-> gap.from) {
            i = gb->gap.to - 1;
        }
        printf("%c", gb->buffer[i]);
    }
    printf("\n");
}

void gap_buffer_save_to_file(const GapBuffer *gb, const char *filename) {//done
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        fwrite(gb->buffer, 1, gb->gap.from, file);
        fwrite(gb->buffer + gb->gap.to, 1, gb->length - gb->gap.to, file);
        fclose(file);
        printf("Text has been saved successfully\n");
    } else {
        printf("Error opening file for writing\n");
    }
}

void gap_buffer_load_from_file(GapBuffer *gb, const char *filename) {//done
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        gb->length = file_size + DEFAULT_WINDOW_SIZE;
        gb->buffer = (char *)realloc(gb->buffer, gb->length * sizeof(char));
        fread(gb->buffer, 1, file_size, file);
        fclose(file);

        gb->gap.from = file_size;
        gb->gap.to = gb->length;
        printf("Text has been loaded successfully\n");
    } else {
        printf("Error opening file for reading\n");
    }
}

void gap_buffer_insert_at(GapBuffer *gb, int line, int index, const char *text) {
    // TODO: Add multi-line support
    int position = index;
    move_gap(gb, position);
    gap_buffer_insert_text(gb, text);
}

void gap_buffer_search(const GapBuffer *gb, const char *text) {//done
    int text_length = strlen(text);
    for (int i = 0; i <= gb->gap.from - text_length; i++) {
        if (strncmp(gb->buffer + i, text, text_length) == 0) {
            printf("Text is present in this position: %d %d\n", 0, i);
        }
    }
    for (int i = gb->gap.to; i <= gb->length - text_length; i++) {
        if (strncmp(gb->buffer + i, text, text_length) == 0) {
            printf("Text is present in this position: %d %d\n",
                0, i - (gb->gap.to - gb->gap.from));
        }
    }
}
