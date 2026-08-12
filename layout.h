#ifndef LAYOUT_H

#include <stdbool.h>

#if __STDC_VERSION__ < 201112L
    #error "This C version is not be supported"
#elif __STDC_VERSION__ < 202311L
    typedef char u8_t;
#else
    typedef char8_t u8_t;
#endif



typedef void* layout_handler_t;

#define LAYOUT_BASIC_ELEM_SIZE (3)    // This is for the elements which we used

layout_handler_t initialize_layout(int word_len, int word_num);
void destroy_layout(layout_handler_t layout_handler);
const u8_t* get_layout(layout_handler_t layout_handler);
bool draw_layout(layout_handler_t layout_handler);
bool add_word(layout_handler_t layout_handler, const char* word, int order);

#endif // LAYOUT_H