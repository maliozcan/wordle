#include <stdio.h>
#include <string.h> // remove

#include "layout.h"

int main(int argc, char* arg[]) {
    printf("Welcome to wordle!\n");
 

    layout_handler_t layout_handler = initialize_layout(3, 1);
    if (NULL == layout_handler) {
        fprintf(stderr, "The layout didn't initialized\n");
        return 1;
    }

    draw_layout(layout_handler);

    destroy_layout(layout_handler);


    u8_t arr[12] = {0};


    memcpy(arr, "─", 3);
    memcpy(arr + 3, "─", 3);

    printf("arr: %s\n", arr);

    return 0;
}