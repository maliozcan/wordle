#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "layout.h"

#ifdef DEBUG
#define log(...)    printf(__VA_ARGS__)
#else
#define log(...)
#endif

// It must get more than one argument
#define TODO(fmt, ...) fprintf(stderr, "TODO: " fmt , __VA_ARGS__)

#define SQUARE_SIZE     (5)
#define INNER_SIZE      (3)
#define VERTICAL_SIZE   (3)

// TODO: remove it if it is not used
// typedef struct {
//     int size;
//     u8_t data[4];
// } UTF8;

// Use _Static_assert to check size of ue literals
const u8_t LIGHT_HORIZONTAL[LAYOUT_BASIC_ELEM_SIZE]        = u8"─"; // U+2500, \u2500
const u8_t LIGHT_VERTICAL[LAYOUT_BASIC_ELEM_SIZE]          = u8"│"; // U+2502, \u2502
const u8_t DOWN_AND_RIGHT[LAYOUT_BASIC_ELEM_SIZE]          = u8"┌"; // U+250C, \u250C
const u8_t DOWN_AND_LEFT[LAYOUT_BASIC_ELEM_SIZE]           = u8"┐"; // U+2510, \u2510
const u8_t UP_AND_RIGHT[LAYOUT_BASIC_ELEM_SIZE]            = u8"└"; // U+2514, \u2514
const u8_t UP_AND_LEFT[LAYOUT_BASIC_ELEM_SIZE]             = u8"┘"; // U+2518, \u2518
const u8_t VERTICAL_AND_RIGHT[LAYOUT_BASIC_ELEM_SIZE]      = u8"├"; // U+251C, \u251C
const u8_t VERTICAL_AND_LEFT[LAYOUT_BASIC_ELEM_SIZE]       = u8"┤"; // U+2524, \u2524
const u8_t DOWN_AND_HORIZONTAL[LAYOUT_BASIC_ELEM_SIZE]     = u8"┬"; // U+252C, \u252C
const u8_t UP_AND_HORIZONTAL[LAYOUT_BASIC_ELEM_SIZE]       = u8"┴"; // U+2534, \u2534
const u8_t VERTICAL_AND_HORIZONTAL[LAYOUT_BASIC_ELEM_SIZE] = u8"┼"; // U+253C, \u253C

typedef struct {
    int word_len;
    int word_num;
    size_t buffer_size;
    u8_t* buffer;
    size_t used_size;
} layout_handler_impl_t;

bool prapare_layout(layout_handler_impl_t* layout_handler_impl);

// static inline
// void set_utf8(UTF8* utf8, const u8_t* c, const size_t size)
// {
//     assert(size <= 4LU);
//     memcpy(utf8->data, c, size);
//     utf8->size = size;
// }

layout_handler_t initialize_layout(int word_len, int word_num)
{
    assert(word_len > 2);
    assert(word_num > 0);

    // const size_t line_size = LAYOUT_BASIC_ELEM_SIZE * (SQUARE_SIZE + (word_len - 1) * (SQUARE_SIZE - 1)) + 1 /*This is for newline*/;
    const size_t line_size = LAYOUT_BASIC_ELEM_SIZE * (SQUARE_SIZE + (word_len - 1) * (SQUARE_SIZE - 1)) + 1 /*This is for newline*/;
    const size_t inner_size = LAYOUT_BASIC_ELEM_SIZE * (word_len + 1) + INNER_SIZE * word_len + 1 /*This is for newline*/;
    const size_t total_size = line_size * (word_num + 1) + inner_size * word_num;
    log("word_len %d word_num %d line_size %zu inner_size %zu total_size %zu\n", word_len, word_num, line_size, inner_size, total_size);

    layout_handler_impl_t* layout_handler_impl = calloc(1, sizeof(layout_handler_impl_t));
    if (NULL == layout_handler_impl) {
        return NULL;
    }
    u8_t* buffer = calloc(total_size, sizeof(u8_t));
    if (NULL == buffer) {
        free(layout_handler_impl);
        return NULL;
    }
    
    // layout_handler_t layout_handler
    layout_handler_impl->word_len = word_len;
    layout_handler_impl->word_num = word_num;
    layout_handler_impl->buffer_size = total_size;
    layout_handler_impl->buffer = buffer;

    prapare_layout(layout_handler_impl);

    return layout_handler_impl;
}

void destroy_layout(layout_handler_t layout_handler)
{
    layout_handler_impl_t* layout_handler_impl = (layout_handler_impl_t*) layout_handler;
    if (NULL == layout_handler_impl) {
        return;
    }
    if (layout_handler_impl->buffer) {
        free(layout_handler_impl->buffer);
    }
    free(layout_handler_impl);
}

void append_buffer(layout_handler_impl_t* layout_handler_impl, const u8_t* str, const size_t size)
{
    memcpy(layout_handler_impl->buffer + layout_handler_impl->used_size, str, size);
    layout_handler_impl->used_size += size;
}

#define append_newline(layout_handler_impl)    append_buffer(layout_handler_impl, "\n", 1)

bool prapare_layout(layout_handler_impl_t* layout_handler_impl)
{
    if (NULL == layout_handler_impl) {
        fprintf(stderr, "Drawer was not initialized\n");
        return false;
    }

    const int word_len = layout_handler_impl->word_len;
    const int word_num = layout_handler_impl->word_num;

    const int char_num_in_line = word_len * (SQUARE_SIZE - 1) + 1;
    const int char_num_in_column = word_num * (VERTICAL_SIZE - 1) + 1;
    
    // row 0
    append_buffer(layout_handler_impl, "┌", LAYOUT_BASIC_ELEM_SIZE);
    for (int col = 1; col != char_num_in_line - 1; ++col) {
        const u8_t* c = (col % 4 == 0) ? "┬" : "─";
        append_buffer(layout_handler_impl, c, LAYOUT_BASIC_ELEM_SIZE);
    }
    append_buffer(layout_handler_impl, "┐", LAYOUT_BASIC_ELEM_SIZE);
    append_newline(layout_handler_impl);

    for (int row = 1; row != char_num_in_column - 1; ++row) {
        for (int col = 0; col != char_num_in_line; ++col) {
            if ((row - 1) % 2 == 0) { // middle
                const u8_t* c = (col % 4 == 0) ? "│" : " ";
                const size_t size = (col % 4 == 0) ? LAYOUT_BASIC_ELEM_SIZE : 1;
                append_buffer(layout_handler_impl, c, size);
            } else {
                assert(row % 2 == 0);
                if (0 == col) {
                    append_buffer(layout_handler_impl, "├", LAYOUT_BASIC_ELEM_SIZE);
                } else if (char_num_in_line - 1 == col) {
                    append_buffer(layout_handler_impl, "┤", LAYOUT_BASIC_ELEM_SIZE);
                } else {
                    const u8_t* c = (col % 4 == 0) ? "┼" : "─";
                    append_buffer(layout_handler_impl, c, LAYOUT_BASIC_ELEM_SIZE);
                }
            }
        }
        append_newline(layout_handler_impl);
    }
 
    // row (char_num_in_column - 1)
    append_buffer(layout_handler_impl, "└", LAYOUT_BASIC_ELEM_SIZE);
    for (int col = 1; col != char_num_in_line - 1; ++col) {
        const u8_t* c = (col % 4 == 0) ? "┴" : "─";
        append_buffer(layout_handler_impl, c, LAYOUT_BASIC_ELEM_SIZE);
    }
    append_buffer(layout_handler_impl, "┘", LAYOUT_BASIC_ELEM_SIZE);
    append_newline(layout_handler_impl);

    return true;
}

bool draw_layout(layout_handler_t layout_handler)
{
    layout_handler_impl_t* layout_handler_impl = (layout_handler_impl_t*) layout_handler;
    if (NULL == layout_handler_impl) {
        fprintf(stderr, "Layout was not initialized\n");
        return false;
    }
    printf("%s\n", layout_handler_impl->buffer);
    return true;
}

const u8_t* get_layout(layout_handler_t layout_handler)
{
    layout_handler_impl_t* layout_handler_impl = (layout_handler_impl_t*) layout_handler;
    if (NULL == layout_handler_impl) {
        fprintf(stderr, "Layout was not initialized\n");
        return false;
    }
    return layout_handler_impl->buffer;
}

const char* draw_example()
{
    static char box_1x1[] =
        "┌───┐\n"
        "│ A │\n"
        "└───┘\n";
    (void) box_1x1;

    static char box_8x8[] =
        "┌───┬───┬───┬───┬───┬───┬───┬───┐\n"
        "│ A │ B │ C │   │   │   │   │   │\n"
        "├───┼───┼───┼───┼───┼───┼───┼───┤\n"
        "│   │   │   │   │   │   │   │   │\n"
        "├───┼───┼───┼───┼───┼───┼───┼───┤\n"
        "│   │   │   │   │   │   │   │   │\n"
        "├───┼───┼───┼───┼───┼───┼───┼───┤\n"
        "│   │   │   │   │   │   │   │   │\n"
        "├───┼───┼───┼───┼───┼───┼───┼───┤\n"
        "│   │   │   │   │   │   │   │   │\n"
        "├───┼───┼───┼───┼───┼───┼───┼───┤\n"
        "│   │   │   │   │   │   │   │   │\n"
        "├───┼───┼───┼───┼───┼───┼───┼───┤\n"
        "│   │   │   │   │   │   │   │   │\n"
        "├───┼───┼───┼───┼───┼───┼───┼───┤\n"
        "│   │   │   │   │   │   │   │   │\n"
        "└───┴───┴───┴───┴───┴───┴───┴───┘\n";


    printf("Sizeof 1x1 = %zu\n", sizeof(box_1x1));
    printf("Sizeof 8x8 = %zu\n", sizeof(box_8x8));
    printf("sizeof(\"├───┼───┼───┼───┼───┼───┼───┼───┤\") = %zu\n", sizeof("├───┼───┼───┼───┼───┼───┼───┼───┤"));
    printf("sizeof(\"│   │   │   │   │   │   │   │   │\") = %zu\n", sizeof("│   │   │   │   │   │   │   │   │"));

    return box_8x8;
}

