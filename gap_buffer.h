#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H
#include <stdbool.h>

typedef struct {
    int from;
    int to;
} GapWindow;

typedef struct {
    int length;
    GapWindow gap;
    char *buffer;
    char *filepath;
    bool is_saved;
    int *line_starts;
    int line_count;
} GapBuffer;

void gap_buffer_init(GapBuffer *gb);
void gap_buffer_free(GapBuffer *gb);
void gap_buffer_append(GapBuffer *gb, const char *text);
void gap_buffer_delete_at(GapBuffer *gb, int line, int index, int length);
void gap_buffer_print(const GapBuffer *gb);
void gap_buffer_save_to_file(GapBuffer *gb, const char *filename);
void gap_buffer_load_from_file(GapBuffer *gb, const char *filename);
void gap_buffer_insert_at(GapBuffer *gb, int line, int index, const char *text);
void gap_buffer_search(const GapBuffer *gb, const char *text);

#endif //GAP_BUFFER_H


