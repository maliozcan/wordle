#include <string.h>
#include <locale.h>
#include <wchar.h>

#include "utest.h"

#include "layout.h"

UTEST_STATE();

void configure_locale()
{
    const char* english_code = "en_US.UTF-8";
    const char* l;
    if ((l = setlocale(LC_ALL, english_code)) == NULL) {
        fprintf(stderr, "Couldn't set locale as %s\n", english_code);
        exit(1);
    }
    printf("locale: %s\n", l);
}

int main(int argc, const char *const argv[]) {
    configure_locale();
    return utest_main(argc, argv);
}

static const letter_position_type position[MAX_WORD_LENGTH] = {0};

UTEST(layout_initializtion, empty_layout_3x1) {
    layout_handler_t layout_handler = initialize_layout(3, 1);
    ASSERT_TRUE(NULL != layout_handler);

    size_t line_num = 0;
    const size_t num_of_bytes_written = draw_layout(layout_handler, &line_num);
    char* layout = malloc(num_of_bytes_written);
    get_layout(layout, num_of_bytes_written, layout_handler);

    printf("LAYOUT: \n%s", layout);

    static char box_3x1[] =
        "┌───┬───┬───┐\n"
        "│   │   │   │\n"
        "└───┴───┴───┘\n";

    printf("num_of_bytes_written = %lu, sizeof box_3x1 = %lu\n", num_of_bytes_written, sizeof(box_3x1));
    ASSERT_EQ(strcmp(layout, box_3x1), 0);

    destroy_layout(layout_handler);
    free(layout);
}

UTEST(layout_initializtion, empty_layout_5x1) {
    layout_handler_t layout_handler = initialize_layout(5, 1);
    ASSERT_TRUE(NULL != layout_handler);

    size_t line_num = 0;
    const size_t num_of_bytes_written = draw_layout(layout_handler, &line_num);
    char* layout = malloc(num_of_bytes_written);
    get_layout(layout, num_of_bytes_written, layout_handler);

    static char box_5x1[] =
        "┌───┬───┬───┬───┬───┐\n"
        "│   │   │   │   │   │\n"
        "└───┴───┴───┴───┴───┘\n";

    ASSERT_EQ(strcmp(layout, box_5x1), 0);

    destroy_layout(layout_handler);
    free(layout);
}

UTEST(layout_initializtion, empty_layout_5x2) {
    layout_handler_t layout_handler = initialize_layout(5, 2);
    ASSERT_TRUE(NULL != layout_handler);

    size_t line_num = 0;
    const size_t num_of_bytes_written = draw_layout(layout_handler, &line_num);
    char* layout = malloc(num_of_bytes_written);
    get_layout(layout, num_of_bytes_written, layout_handler);

    static char box_5x2[] =
        "┌───┬───┬───┬───┬───┐\n"
        "│   │   │   │   │   │\n"
        "├───┼───┼───┼───┼───┤\n"
        "│   │   │   │   │   │\n"
        "└───┴───┴───┴───┴───┘\n";

    ASSERT_EQ(strcmp(layout, box_5x2), 0);

    destroy_layout(layout_handler);
    free(layout);
}

UTEST(layout_initializtion, empty_layout_5x3) {
    layout_handler_t layout_handler = initialize_layout(5, 3);
    ASSERT_TRUE(NULL != layout_handler);

    size_t line_num = 0;
    const size_t num_of_bytes_written = draw_layout(layout_handler, &line_num);
    char* layout = malloc(num_of_bytes_written);
    get_layout(layout, num_of_bytes_written, layout_handler);


    static char box_5x3[] =
        "┌───┬───┬───┬───┬───┐\n"
        "│   │   │   │   │   │\n"
        "├───┼───┼───┼───┼───┤\n"
        "│   │   │   │   │   │\n"
        "├───┼───┼───┼───┼───┤\n"
        "│   │   │   │   │   │\n"
        "└───┴───┴───┴───┴───┘\n";

    ASSERT_EQ(strcmp(layout, box_5x3), 0);

    destroy_layout(layout_handler);
    free(layout);
}

UTEST(word_addition, layout_5x1) {
    layout_handler_t layout_handler = initialize_layout(5, 1);
    ASSERT_TRUE(NULL != layout_handler);

    size_t line_num = 0;
    draw_layout(layout_handler, &line_num);
    ASSERT_TRUE(add_word(layout_handler, L"table", 0, position));

    const size_t num_of_bytes_written = draw_layout(layout_handler, &line_num);
    char* layout = malloc(num_of_bytes_written);
    get_layout(layout, num_of_bytes_written, layout_handler);

    static char box_5x1[] =
    "┌───┬───┬───┬───┬───┐\n"
    "│ T │ A │ B │ L │ E │\n"
    "└───┴───┴───┴───┴───┘\n";

    ASSERT_EQ(strcmp(layout, box_5x1), 0);

    destroy_layout(layout_handler);
    free(layout);
}

UTEST(word_addition, layout_5x2) {
    layout_handler_t layout_handler = initialize_layout(5, 2);
    ASSERT_TRUE(NULL != layout_handler);

    size_t line_num = 0;
    draw_layout(layout_handler, &line_num);
    ASSERT_TRUE(add_word(layout_handler, L"Table", 0, position));
    ASSERT_TRUE(add_word(layout_handler, L"SiGHt", 1, position));
    const size_t num_of_bytes_written = draw_layout(layout_handler, &line_num);
    char* layout = malloc(num_of_bytes_written);
    get_layout(layout, num_of_bytes_written, layout_handler);

    static char box_5x2[] =
        "┌───┬───┬───┬───┬───┐\n"
        "│ T │ A │ B │ L │ E │\n"
        "├───┼───┼───┼───┼───┤\n"
        "│ S │ I │ G │ H │ T │\n"
        "└───┴───┴───┴───┴───┘\n";

    ASSERT_EQ(strcmp(layout, box_5x2), 0);

    destroy_layout(layout_handler);
    free(layout);
}

UTEST(word_addition, layout_5x3) {
    layout_handler_t layout_handler = initialize_layout(5, 3);
    ASSERT_TRUE(NULL != layout_handler);

    size_t line_num = 0;
    draw_layout(layout_handler, &line_num);
    ASSERT_TRUE(add_word(layout_handler, L"Table", 0, position));
    ASSERT_TRUE(add_word(layout_handler, L"SIGHT", 1, position));
    ASSERT_TRUE(add_word(layout_handler, L"radar", 2, position));
    const size_t num_of_bytes_written = draw_layout(layout_handler, &line_num);
    char* layout = malloc(num_of_bytes_written);
    get_layout(layout, num_of_bytes_written, layout_handler);

    static char box_5x3[] =
        "┌───┬───┬───┬───┬───┐\n"
        "│ T │ A │ B │ L │ E │\n"
        "├───┼───┼───┼───┼───┤\n"
        "│ S │ I │ G │ H │ T │\n"
        "├───┼───┼───┼───┼───┤\n"
        "│ R │ A │ D │ A │ R │\n"
        "└───┴───┴───┴───┴───┘\n";

    ASSERT_EQ(strcmp(layout, box_5x3), 0);

    destroy_layout(layout_handler);
    free(layout);
}
