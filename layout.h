#ifndef LAYOUT_H

#include <stdbool.h>

#include "common_defs.h"

typedef void* layout_handler_t;

layout_handler_t initialize_layout(int word_len, int word_num);
void destroy_layout(layout_handler_t layout_handler);
const u8_t* get_layout(layout_handler_t layout_handler);
bool draw_layout(layout_handler_t layout_handler);
bool add_word(layout_handler_t layout_handler, const char* word, int order);
void clear_layout(layout_handler_t layout_handler);

#endif // LAYOUT_H