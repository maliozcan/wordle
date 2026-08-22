#include <string.h>

#include "utest.h"

#include "layout.h"

UTEST_MAIN()

UTEST(layout_initializtion, empty_layout_3x1) {
    layout_handler_t layout_handler = initialize_layout(3, 1);
    ASSERT_TRUE(NULL != layout_handler);

    const size_t num_of_bytes_written = draw_layout(layout_handler);
    u8_t* layout = malloc(num_of_bytes_written);
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

    const size_t num_of_bytes_written = draw_layout(layout_handler);
    u8_t* layout = malloc(num_of_bytes_written);
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

    const size_t num_of_bytes_written = draw_layout(layout_handler);
    u8_t* layout = malloc(num_of_bytes_written);
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

    const size_t num_of_bytes_written = draw_layout(layout_handler);
    u8_t* layout = malloc(num_of_bytes_written);
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

    draw_layout(layout_handler);
    ASSERT_TRUE(add_word(layout_handler, "table", 0));

    const size_t num_of_bytes_written = draw_layout(layout_handler);
    u8_t* layout = malloc(num_of_bytes_written);
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

    draw_layout(layout_handler);
    ASSERT_TRUE(add_word(layout_handler, "Table", 0));
    ASSERT_TRUE(add_word(layout_handler, "SiGHt", 1));
    const size_t num_of_bytes_written = draw_layout(layout_handler);
    u8_t* layout = malloc(num_of_bytes_written);
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

    draw_layout(layout_handler);
    ASSERT_TRUE(add_word(layout_handler, "Table", 0));
    ASSERT_TRUE(add_word(layout_handler, "SIGHT", 1));
    ASSERT_TRUE(add_word(layout_handler, "radar", 2));
    const size_t num_of_bytes_written = draw_layout(layout_handler);
    u8_t* layout = malloc(num_of_bytes_written);
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
