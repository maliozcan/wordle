#include <string.h>

#include "utest.h"

#include "layout.h"

UTEST_MAIN()

UTEST(layout_initializtion, empty_layout_3x1) {
    layout_handler_t layout_handler = initialize_layout(3, 1);
    ASSERT_TRUE(NULL != layout_handler);

    draw_layout(layout_handler);

    const u8_t* layout = get_layout(layout_handler);

    static char box_3x1[] =
        "┌───┬───┬───┐\n"
        "│   │   │   │\n"
        "└───┴───┴───┘\n";

    ASSERT_EQ(strcmp(layout, box_3x1), 0);

    destroy_layout(layout_handler);
}

UTEST(layout_initializtion, empty_layout_5x1) {
    layout_handler_t layout_handler = initialize_layout(5, 1);
    ASSERT_TRUE(NULL != layout_handler);

    draw_layout(layout_handler);

    const u8_t* layout = get_layout(layout_handler);

    static char box_5x1[] =
        "┌───┬───┬───┬───┬───┐\n"
        "│   │   │   │   │   │\n"
        "└───┴───┴───┴───┴───┘\n";

    ASSERT_EQ(strcmp(layout, box_5x1), 0);

    destroy_layout(layout_handler);
}

UTEST(layout_initializtion, empty_layout_5x2) {
    layout_handler_t layout_handler = initialize_layout(5, 2);
    ASSERT_TRUE(NULL != layout_handler);
    const u8_t* layout = get_layout(layout_handler);
    draw_layout(layout_handler);

    static char box_5x2[] =
        "┌───┬───┬───┬───┬───┐\n"
        "│   │   │   │   │   │\n"
        "├───┼───┼───┼───┼───┤\n"
        "│   │   │   │   │   │\n"
        "└───┴───┴───┴───┴───┘\n";

    ASSERT_EQ(strcmp(layout, box_5x2), 0);

    destroy_layout(layout_handler);
}


UTEST(layout_initializtion, empty_layout_5x3) {
    layout_handler_t layout_handler = initialize_layout(5, 3);
    ASSERT_TRUE(NULL != layout_handler);
    const u8_t* layout = get_layout(layout_handler);

    draw_layout(layout_handler);


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
}
