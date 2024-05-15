#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER_SIZE 100

const int DEFAULT_WINDOW_SIZE = 10;

typedef struct {
    int from;
    int to;
} GapWindow;

typedef struct {
    int length;
    GapWindow gap;
    char *buffer;
} GapBuffer;

void gap_buffer_init(GapBuffer *gb) {
    gb->buffer = (char *)malloc(INITIAL_BUFFER_SIZE * sizeof(char));
    gb->length = INITIAL_BUFFER_SIZE;
    gb->gap.from = 0;
    gb->gap.to = INITIAL_BUFFER_SIZE;
}

void gap_buffer_free(GapBuffer *buffer) {
    free(buffer->buffer);
    free(buffer);
}

int gap_get_length(const GapWindow *gap) {
    return gap->to - gap->from;
}

void gap_buffer_insert_text(GapBuffer *gb, const char *text) {
    int text_length = strlen(text);
    if (text_length > gb->gap.to - gb->gap.from) {
        gb->length += text_length + DEFAULT_WINDOW_SIZE;
        gb->buffer = realloc(gb->buffer, gb->length * sizeof(char));
    }

    strncpy(gb->buffer + gb->gap.from, text, text_length);
    gb->gap.from += text_length;
    gb->gap.to += text_length;
}

void move_gap(GapBuffer *gb, int position) {
    if (position < gb->gap.from) {
        memmove(
            gb->buffer + position + gap_get_length(&gb->gap),
            gb->buffer + position,
            gb->gap.from - position);

        gb->gap.to = position + gap_get_length(&gb->gap);
        gb->gap.from = position;
    } else if (position > gb->gap.to) {
        memmove(
            gb->buffer + gb->gap.from,
            gb->buffer + gb->gap.to,
            position - gb->gap.to);

        gb->gap.from += position - gb->gap.to;
        gb->gap.to = position;
    }
}

void gap_buffer_delete_text(GapBuffer *gb, int position, int length) {
    if (length > 0) {
        move_gap(gb, position);
        gb->gap.from -= length;
    }
}

void gap_buffer_print(const GapBuffer *gb) {
    for (int i = 0; i < gb->length; i++) {
        if (i == gb->gap.from) {
            printf("|");
        }
        if (i == gb->gap.to) {
            printf("|");
        }
        printf("%c", gb->buffer[i]);
    }
    printf("\n");
}


int main(void) {

    GapBuffer *gb = (GapBuffer *)malloc(sizeof(GapBuffer));

    gap_buffer_init(gb);

    gap_buffer_insert_text(gb, "Hello, ");
    gap_buffer_insert_text(gb, "world!");

    gap_buffer_delete_text(gb, 0, 1);
    gap_buffer_print(gb);

    gap_buffer_free(gb);
    return 0;
}
