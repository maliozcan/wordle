#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <wctype.h>

#include "layout.h"
#include "helper.h"

#define SQUARE_SIZE     (5)
#define INNER_SIZE      (3)
#define VERTICAL_SIZE   (3)


typedef enum {
    COLOR_DEFAULT,
    COLOR_GOLDEN,
    COLOR_GREEN,
} color_type;

static const char* default_color = "\033[0m";
static const char* green_color = "\033[32m";
static const char* golden_color = "\033[38;5;220m";

typedef struct {
    color_type color;
    wchar_t data;
} letter_type;

typedef struct {
    int word_num;
    int word_length;
    letter_type letter[MAX_ROW_NUM][MAX_WORD_LENGTH];
} layout_structure_type;

static size_t _prapare_layout(FILE* filestream, layout_structure_type* layout_structure, size_t* line_num);
static color_type _get_color(const letter_position_type position);

static void set_letter(letter_type* letter, color_type color, const wchar_t c)
{
    assert(letter);
    letter->color = color;;
    letter->data = c;
}

layout_handler_t initialize_layout(int word_len, int word_num)
{
    assert(word_len > 2);
    assert(word_num > 0);

    layout_structure_type* layout_structure = calloc(1, sizeof(layout_structure_type));
    if (NULL == layout_structure) {
        return NULL;
    }

    layout_structure->word_length = word_len;
    layout_structure->word_num = word_num;

    for (int row_index = 0; row_index != MAX_ROW_NUM; ++row_index) {
        for (int letter_index = 0; letter_index != MAX_WORD_LENGTH; ++letter_index) {
            set_letter(&layout_structure->letter[row_index][letter_index], COLOR_DEFAULT, L' ');
        }
    }

    return layout_structure;
}

void destroy_layout(layout_handler_t layout_handler)
{
    layout_structure_type* layout_structure = (layout_structure_type*) layout_handler;
    if (NULL == layout_structure) {
        return;
    }
    free(layout_structure);
}

size_t print_letter(FILE* f, const letter_type* letter)
{
    char all_buffer[32];
    switch (letter->color) {
        case COLOR_DEFAULT:
            snprintf(all_buffer, sizeof(all_buffer), "%lc", letter->data);
            break;
        case COLOR_GREEN:
            snprintf(all_buffer, sizeof(all_buffer), "%s%lc%s", green_color, letter->data, default_color);
            break;
        case COLOR_GOLDEN:
            snprintf(all_buffer, sizeof(all_buffer), "%s%lc%s", golden_color, letter->data, default_color);
            break;
        default:
            break;
    }
    fprintf(f, "%s", all_buffer);
    return strlen(all_buffer);
}

static size_t _prapare_layout(FILE* filestream, layout_structure_type* layout_structure, size_t* line_num)
{
    if (NULL == layout_structure) {
        fprintf(stderr, "Layout was not initialized\n");
        return 0;
    }

    const int word_len = layout_structure->word_length;
    const int word_num = layout_structure->word_num;

    const int char_num_in_line = word_len * (SQUARE_SIZE - 1) + 1;
    const int char_num_in_column = word_num * (VERTICAL_SIZE - 1) + 1;

    size_t character_num = 0;

    // row 0
    character_num += print(filestream, L"┌", 1, line_num);
    for (int col = 1; col != char_num_in_line - 1; ++col) {
        const wchar_t* c = (col % 4 == 0) ? L"┬" : L"─";
        character_num += print(filestream, c, 1, line_num);
    }
    character_num += print(filestream, L"┐", 1, line_num);
    character_num += print_newline(filestream, line_num);

    for (int row = 1; row != char_num_in_column - 1; ++row) {
        for (int col = 0; col != char_num_in_line; ++col) {
            if ((row - 1) % 2 == 0) { // middle
                if (col % 4 == 0) {
                    character_num += print(filestream, L"│", 1, line_num);
                } else if (col % 4 == 2) {
                    // Print letter
                    // character_num += print(filestream, "X", 1);
                    assert(row % 2 == 1);
                    const int word_order = (row - 1) / 2;
                    const int letter_index = (col - 2) / 4;
                    character_num += print_letter(filestream, &layout_structure->letter[word_order][letter_index]);
                } else {
                    character_num += print(filestream, L" ", 1, line_num);
                }
            } else {
                assert(row % 2 == 0);
                if (0 == col) {
                    character_num += print(filestream, L"├", 1, line_num);
                } else if (char_num_in_line - 1 == col) {
                    character_num += print(filestream, L"┤", 1, line_num);
                } else {
                    const wchar_t* c = (col % 4 == 0) ? L"┼" : L"─";
                    character_num += print(filestream, c, 1, line_num);
                }
            }
        }
        character_num += print_newline(filestream, line_num);
    }
 
    // row (char_num_in_column - 1)
    character_num += print(filestream, L"└", 1, line_num);
    for (int col = 1; col != char_num_in_line - 1; ++col) {
        const wchar_t* c = (col % 4 == 0) ? L"┴" : L"─";
        character_num += print(filestream, c, 1, line_num);
    }
    character_num += print(filestream, L"┘", 1, line_num);
    character_num += print_newline(filestream, line_num);

    return character_num;
}

size_t draw_layout(layout_handler_t layout_handler, size_t* line_num)
{
    layout_structure_type* layout_structure = (layout_structure_type*) layout_handler;
    if (NULL == layout_structure) {
        fprintf(stderr, "Layout was not initialized\n");
        return 0;
    }
    return _prapare_layout(stdout, layout_structure, line_num) * sizeof(wchar_t);
}

bool get_layout(char* buffer, const size_t buf_size, layout_handler_t layout_handler)
{
    layout_structure_type* layout_structure = (layout_structure_type*) layout_handler;
    if (NULL == layout_structure) {
        fprintf(stderr, "Layout was not initialized\n");
        return 0;
    }
    FILE* tmpf = tmpfile();
    if (!tmpf) {
        perror("tmpfile_f failed");
        return false;
    }
    size_t line_num = 0;
    if (_prapare_layout(tmpf, layout_structure, &line_num) == 0) {
        return false;
    }
    rewind(tmpf);

    char line[128];
    size_t total = 0;
    while(fgets(line, sizeof(line), tmpf)) {
        const size_t size = strlen(line);
        if (total + size <= buf_size) {
            memcpy(buffer + total, line, size);
            total += size;
        }
    }

    return true;
}

bool add_word(layout_handler_t layout_handler, const wchar_t* word, int order, const letter_position_type position[MAX_WORD_LENGTH])
{
    layout_structure_type* layout_structure = (layout_structure_type*) layout_handler;
    if (NULL == layout_structure) {
        fprintf(stderr, "Layout was not initialized\n");
        return 0;
    }
    if (layout_structure->word_num <= order || 0 > order) {
        fprintf(stderr, "order (%d) cannot be greater than or equal to word_num %d\n", order, layout_structure->word_num);
        return false;
    }
    const int word_len = layout_structure->word_length;
    if (wcslen(word) != word_len) {
        fprintf(stderr, "The length of the word (%ls) has to be %d\n", word, word_len);
        return false;
    }

    for (int i = 0; i != word_len; ++i) {
        const color_type color = _get_color(position[i]);
        set_letter(&layout_structure->letter[order][i], color, towupper(word[i]));
    }
    return true;
}

static color_type _get_color(const letter_position_type position)
{
    _Static_assert((int) NONE == COLOR_DEFAULT, "");
    _Static_assert((int) EXIST == COLOR_GOLDEN, "");
    _Static_assert((int) RIGHT_PLACE == COLOR_GREEN, "");

    static const color_type position_to_color[NUM_OF_LETTER_POSITION] = {COLOR_DEFAULT, COLOR_GOLDEN, COLOR_GREEN};
    if (position >= NONE && position < NUM_OF_LETTER_POSITION) {
        return position_to_color[position];
    }
    return COLOR_DEFAULT;
}

void clear_layout(layout_handler_t layout_handler)
{
    layout_structure_type* layout_structure = (layout_structure_type*) layout_handler;
    if (NULL == layout_structure) {
        fprintf(stderr, "Layout was not initialized\n");
        return;
    }
    const letter_position_type position[MAX_WORD_LENGTH] = {0};
    wchar_t word[MAX_WORD_LENGTH + 1] = {0};
    for (int i = 0; i != layout_structure->word_length; ++i) {
        word[i] = L' ';
    }
    for (int order = 0; order != layout_structure->word_num; ++order) {
        add_word(layout_handler, word, order, position);
    }
}

const char* draw_example(void)
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

