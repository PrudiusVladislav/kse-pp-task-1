#include "gap_buffer.h"
#include "string_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_LINE_STARTS_CAPACITY 10
#define INITIAL_BUFFER_SIZE 100
#define DEFAULT_WINDOW_SIZE 10

void gap_buffer_init(GapBuffer *gb) {
    gb->buffer = (char *)malloc((INITIAL_BUFFER_SIZE + 1) * sizeof(char));
    gb->length = INITIAL_BUFFER_SIZE;
    gb->buffer[gb->length] = '\0';

    gb->gap.from = 0;
    gb->gap.to = INITIAL_BUFFER_SIZE;

    gb->filepath = NULL;
    gb->is_saved = true;

    gb->line_starts = (LineStart*)malloc(INITIAL_LINE_STARTS_CAPACITY * sizeof(LineStart));
    gb->line_starts_capacity = INITIAL_LINE_STARTS_CAPACITY;
    gb->line_count = 0;
}

int gap_buffer_get_window_length(const GapWindow *gap) {
    return gap->to - gap->from;
}

int gap_buffer_get_content_length(const GapBuffer *gb) {
    return gb->length - gap_buffer_get_window_length(&gb->gap);
}

void gap_buffer_expand(GapBuffer *gb, const int additional_length) {
    const int new_length = gb->length + additional_length + DEFAULT_WINDOW_SIZE;
    char *new_buffer = (char *)malloc((new_length + 1) * sizeof(char));
    terminate_string(new_buffer, new_length);

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

// invoked when a '\n' is inserted or deleted;
// int position - position of the new line start or the line start to delete
void update_line_starts(GapBuffer *gb, const int position, const bool is_insert) {
    if (is_insert) {
        if (gb->line_count == gb->line_starts_capacity) {
            gb->line_starts_capacity *= 2;
            gb->line_starts = (LineStart*)realloc(
                gb->line_starts, gb->line_starts_capacity * sizeof(LineStart));
        }
        int i = gb->line_count;
        while (i > 0 && gb->line_starts[i - 1].position > position) {
            gb->line_starts[i] = gb->line_starts[i - 1];
            i--;
        }
        gb->line_starts[i].position = position;
        gb->line_count++;
    } else {
        int i;
        for (i = 0; i < gb->line_count && gb->line_starts[i].position != position; i++){}
        if (i < gb->line_count) {
            for (; i < gb->line_count - 1; i++) {
                gb->line_starts[i] = gb->line_starts[i + 1];
            }
            gb->line_count--;
        }
    }
}

void move_gap(GapBuffer *gb, const int position) {
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

void gap_buffer_append(GapBuffer *gb, const char *text) {
    //move gap window to the end of the buffer
    move_gap(gb, gap_buffer_get_content_length(gb));

    const int text_length = strlen(text);
    const int gap_length = gap_buffer_get_window_length(&gb->gap);

    if (text_length > gap_length)
        gap_buffer_expand(gb, text_length - gap_length);

    memcpy(gb->buffer + gb->gap.from, text, text_length);
    gb->gap.from += text_length;
    gb->is_saved = false;

    if (gb->line_count == 0) {
        gb->line_starts[gb->line_count].position = 0;
        gb->line_count++;
    }

    if (strchr(text, '\n') != NULL) {
        update_line_starts(gb, gb->gap.from, true);
    }
}

void gap_buffer_print(const GapBuffer *gb) {
    if (gap_buffer_get_content_length(gb) == 0) {
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

void gap_buffer_save_to_file(GapBuffer *gb, const char *filename) {
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

// invoked when loading a file
void populate_line_starts(GapBuffer *gb) {
    gb->line_count = 0;
    for (int i = 0; i < gb->gap.from; i++) {
        if (gb->buffer[i] != '\n') {
            continue;
        }
        if (gb->line_count == gb->line_starts_capacity) {
            gb->line_starts_capacity *= 2;
            gb->line_starts = (LineStart*)realloc(
                gb->line_starts, gb->line_starts_capacity * sizeof(LineStart));
        }
        gb->line_starts[gb->line_count].position = i + 1;
        gb->line_count++;
    }
}

long get_file_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    const long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

void gap_buffer_load_from_file(GapBuffer *gb, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        const long file_size = get_file_size(file);

        gb->length = file_size + DEFAULT_WINDOW_SIZE;
        gb->buffer = (char *)realloc(gb->buffer, (gb->length + 1) * sizeof(char));
        fread(gb->buffer, 1, file_size, file);
        fclose(file);
        terminate_string(gb->buffer, gb->length);

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

void gap_buffer_insert_at(GapBuffer *gb, const int line, const int col, const char *text) {
    if (line < 0 || line >= gb->line_count) {
        printf("Invalid line number\n");
        return;
    }

    const int position = gb->line_starts[line].position + col;
    move_gap(gb, position);

    const int text_length = strlen(text);
    const int gap_length = gap_buffer_get_window_length(&gb->gap);

    if (text_length > gap_length)
        gap_buffer_expand(gb, text_length - gap_length);

    memcpy(gb->buffer + gb->gap.from, text, text_length);
    gb->gap.from += text_length;
    gb->is_saved = false;

    // Update positions of subsequent lines
    for (int i = line + 1; i < gb->line_count; i++) {
        gb->line_starts[i].position += text_length;
    }

    if (strchr(text, '\n') != NULL) {
        update_line_starts(gb, gb->gap.from, true);
    }
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

void gap_buffer_free(GapBuffer *gb) {
    if (gb == NULL) {
        return;
    }

    if (gb->line_starts != NULL) {
        free(gb->line_starts);
    }
    if (gb->buffer != NULL) {
        free(gb->buffer);
    }
    if (gb->filepath != NULL) {
        free(gb->filepath);
    }
    free(gb);
}
