#ifndef LAYOUT_H

#include <stdbool.h>
#include <wchar.h>

#include "common_defs.h"

typedef void* layout_handler_t;

layout_handler_t initialize_layout(int word_len, int word_num);
void destroy_layout(layout_handler_t layout_handler);
bool get_layout(char* buffer, const size_t buf_size, layout_handler_t layout_handler);
size_t draw_layout(layout_handler_t layout_handler, size_t* line_num);
bool add_word(layout_handler_t layout_handler, const wchar_t* word, int order, const letter_position_type position[MAX_WORD_LENGTH]);
void clear_layout(layout_handler_t layout_handler);

#endif // LAYOUT_H