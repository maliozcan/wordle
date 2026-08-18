#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "layout.h"

void empty_stdin(char buf[], const size_t buf_size)
{
    if (buf == NULL) {
        return;
    }
    while (strlen(buf) == buf_size - 1) {
        fgets(buf, buf_size, stdin);
    }
    buf[0] = '\0';
}

typedef int (*fptr_t) (int);

void transform_string(char* str, const size_t size, fptr_t f)
{
    for (size_t i = 0; i != size; ++i) {
        str[i] = f(str[i]);
    }
}

bool ask_user_if_they_want_to_continue(layout_handler_t layout_handler, int* word_order)
{
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("\n");
        return true;
    }
    bool quit = false;
    transform_string(input, strlen(input), tolower);
    if (strncmp(input, "yes", 3) == 0) {
        clear_layout(layout_handler);
        *word_order = 0;
        empty_stdin(input, sizeof(input));
    } else {
        quit = true;
    }
    return quit;
}

int main(int argc, char* arg[]) {
    int word_len = DEFAULT_WORD_LENGTH;
    if (argc == 2) {
        int n = atoi(arg[1]);
        if (n >= MIN_WORD_LENGTH && n <= MAX_WORD_LENGTH) {
            word_len = n;
        } else if (0 != n) {
            fprintf(stderr, "[%d-%d] as word length is supported in this wordle!\n", MIN_WORD_LENGTH, MAX_WORD_LENGTH);
            exit(EXIT_FAILURE);
        } else {
            fprintf(stderr, "Wrong input [%s]!\n", arg[1]);
            exit(EXIT_FAILURE);
        }
    } else {
        // TODO: print help
    }

    printf("Welcome to wordle with %d letters!\n", word_len);

    const int word_num = word_len < 7 ? 6 : 7;
    layout_handler_t layout_handler = initialize_layout(word_len, word_num);
    if (NULL == layout_handler) {
        fprintf(stderr, "The layout didn't initialized\n");
        return 1;
    }

    char target[] = "dummy"; // TODO: get the target word from a database.
    char input[32] = {0};
    char word[MAX_WORD_LENGTH + 1] = {0};
    int word_order = 0;
    bool quit = false;
    while (false == quit) {
        draw_layout(layout_handler);
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break;
        }
        const int str_size = strlen(input);
        if (str_size - 1 == word_len) {
            memcpy(word, input, word_len);
            add_word(layout_handler, word, word_order);
            ++word_order;
            transform_string(word, word_len, tolower);
            if (strncmp(word, target, word_len) == 0) {
                draw_layout(layout_handler);
                printf("You found the word. If you want to continue, type [yes]: ");
                quit = ask_user_if_they_want_to_continue(layout_handler, &word_order);
                if (false == quit) {
                    // TODO: get the target word from a database.
                }
            }
            if (word_num == word_order) {
                draw_layout(layout_handler);
                transform_string(target, word_len, toupper);
                printf("You didn't find the word \"%s\". If you want to continue, type [yes]: ", target);
                quit = ask_user_if_they_want_to_continue(layout_handler, &word_order);
                if (false == quit) {
                    // TODO: get the target word from a database.
                }
            }
        } else {
            printf("Word length should be %d!\n", word_len);
            empty_stdin(input, sizeof(input));
        }
        word[0] = '\0';
    }

    destroy_layout(layout_handler);

    return 0;
}
