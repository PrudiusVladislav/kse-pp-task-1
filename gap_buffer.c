#include "gap_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER_SIZE 100
#define DEFAULT_WINDOW_SIZE 10

void gap_buffer_init(GapBuffer *gb) { //done
    gb->buffer = (char *)malloc((INITIAL_BUFFER_SIZE + 1) * sizeof(char));
    gb->length = INITIAL_BUFFER_SIZE;
    gb->buffer[gb->length] = '\0';

    gb->gap.from = 0;
    gb->gap.to = INITIAL_BUFFER_SIZE;

    gb->filepath = NULL;
    gb->is_saved = true;

    gb->line_starts = (int *)malloc(INITIAL_BUFFER_SIZE * sizeof(int));
    gb->line_count = 0;
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
    char *new_buffer = (char *)malloc((new_length + 1) * sizeof(char));
    new_buffer[new_length] = '\0';

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

void update_line_starts(
    GapBuffer *gb,
    const int position,
    const bool is_insert)
{
    if (is_insert) {
        gb->line_count++;
        gb->line_starts = (int *)realloc(gb->line_starts, gb->line_count * sizeof(int));

        int line = 0;
        while (line < gb->line_count && gb->line_starts[line] < position) {
            line++;
        }

        if (line < gb->line_count) {
            memmove(&gb->line_starts[line + 1], &gb->line_starts[line], (gb->line_count - line - 1) * sizeof(int));
            gb->line_starts[line] = position;
        }
        gb->line_starts[gb->line_count - 1] = position;
    } else {
        if (gb->line_count == 0) {
            return;
        }

        int line = 0;
        while (line < gb->line_count && gb->line_starts[line] < position) {
            line++;
        }

        if (line < gb->line_count) {
            memmove(&gb->line_starts[line], &gb->line_starts[line + 1], (gb->line_count - line - 1) * sizeof(int));
            gb->line_count--;
            gb->line_starts = (int *)realloc(gb->line_starts, gb->line_count * sizeof(int));
        }
    }
}

void gap_buffer_append(GapBuffer *gb, const char *text) {// done
    const int text_length = strlen(text);
    const int gap_length = gap_get_length(&gb->gap);

    if (text_length > gap_length)
        gap_buffer_expand(gb, text_length - gap_length);

    memcpy(gb->buffer + gb->gap.from, text, text_length);
    gb->gap.from += text_length;
    gb->is_saved = false;

    if (strchr(text, '\n') != NULL) {
        update_line_starts(gb, gb->gap.from, true);
    }
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
    gb->is_saved = false;

    if (strchr(gb->buffer + position, '\n') != NULL) {
        update_line_starts(gb, position, false);
    }
}

void gap_buffer_delete_at(GapBuffer *gb, int line, int col, int length) {
    if (line < 0 || line >= gb->line_count) {
        printf("Invalid line number\n");
        return;
    }

    int position = gb->line_starts[line] + col;
    gap_buffer_delete_text(gb, position, length);
}

void gap_buffer_print(const GapBuffer *gb) {//done
    if (gb->length == 0) {
        printf("Buffer is empty :(\n");
        return;
    }

    for (int i = 0; i < gb->gap.from; i++) {
        printf("%c", gb->buffer[i]);
    }
    for (int i = gb->gap.to; i < gb->length; i++) {
        printf("%c", gb->buffer[i]);
    }
    printf("\n");
}

void set_filepath(GapBuffer *gb, const char *filename) {
    free(gb->filepath);
    gb->filepath = strdup(filename);
}

void gap_buffer_save_to_file(GapBuffer *gb, const char *filename) {//done
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        fwrite(gb->buffer, 1, gb->gap.from, file);
        fwrite(gb->buffer + gb->gap.to, 1, gb->length - gb->gap.to, file);
        fclose(file);
        gb->is_saved = true;

        printf("Text has been saved successfully\n");
        set_filepath(gb, filename);
    } else {
        printf("Error opening file for writing\n");
    }
}

void populate_line_starts(GapBuffer *gb) {
    gb->line_count = 0;
    for (int i = 0; i < gb->gap.from; i++) { // assuming that gap window is at the end of the buffer
        if (gb->buffer[i] == '\n') {
            gb->line_starts = (int *)realloc(gb->line_starts, (gb->line_count + 1) * sizeof(int));
            gb->line_starts[gb->line_count] = i + 1;
            gb->line_count++;
        }
    }
}

void gap_buffer_load_from_file(GapBuffer *gb, const char *filename) {//done
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        gb->length = file_size + DEFAULT_WINDOW_SIZE;
        gb->buffer = (char *)realloc(gb->buffer, (gb->length + 1) * sizeof(char));
        fread(gb->buffer, 1, file_size, file);
        fclose(file);
        gb->buffer[gb->length] = '\0';

        gb->gap.from = file_size;
        gb->gap.to = gb->length;
        gb->is_saved = true;

        populate_line_starts(gb);

        printf("Text has been loaded successfully\n");
        set_filepath(gb, filename);
    } else {
        printf("Error opening file for reading\n");
    }
}

void gap_buffer_insert_at(GapBuffer *gb, int line, int col, const char *text) {
    if (line < 0 || line >= gb->line_count) {
        printf("Invalid line number\n");
        return;
    }

    int position = gb->line_starts[line] + col;
    move_gap(gb, position);
    gap_buffer_append(gb, text);
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
