#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H

typedef struct {
    int from;
    int to;
} GapWindow;

typedef struct {
    int length;
    GapWindow gap;
    char *buffer;
} GapBuffer;

void gap_buffer_init(GapBuffer *gb);
void gap_buffer_free(GapBuffer *gb);
int gap_get_length(const GapWindow *gap);
void gap_buffer_expand(GapBuffer *gb, int additional_length);
void gap_buffer_insert_text(GapBuffer *gb, const char *text);
void move_gap(GapBuffer *gb, int position);
void gap_buffer_delete_text(GapBuffer *gb, int position, int length);
void gap_buffer_print(const GapBuffer *gb);
void gap_buffer_save_to_file(const GapBuffer *gb, const char *filename);
void gap_buffer_load_from_file(GapBuffer *gb, const char *filename);
void gap_buffer_insert_at(GapBuffer *gb, int line, int index, const char *text);
void gap_buffer_search(const GapBuffer *gb, const char *text);

#endif //GAP_BUFFER_H


