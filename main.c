#include "helper.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "common_defs.h"
#include "wordle.h"

int main(int argc, char *arg[]) {
    int word_len = DEFAULT_WORD_LENGTH;
    char* first_target = NULL;
    if (argc == 2) {
        const size_t len = strlen(arg[1]); // TODO: use a max lenght.
        if (1 == len) {
            const int n = atoi(arg[1]);
            if (n >= MIN_WORD_LENGTH && n <= MAX_WORD_LENGTH) {
                word_len = n;
            } else if (0 != n) {
                fprintf(stderr, "[%d-%d] as word length is supported in this wordle!\n",
                        MIN_WORD_LENGTH, MAX_WORD_LENGTH);
                exit(EXIT_FAILURE);
            } else {
                fprintf(stderr, "Wrong input [%s]!\n", arg[1]);
                exit(EXIT_FAILURE);
            }
        } else if (len >= MIN_WORD_LENGTH && len <= MAX_WORD_LENGTH) {
            if (validate_word(arg[1], len, len)) {
                first_target = arg[1];
                word_len = len;
            } else {
                fprintf(stderr, "Invalid word [%s]\n", arg[1]);
                exit(EXIT_FAILURE);
            }
        } else {
            fprintf(stderr, "Invlid argument [%s]!\n", arg[1]);
            exit(EXIT_FAILURE);
        }
    } else if (argc > 2) {
        fprintf(stderr, "The number of argument [%d] is wrong.\n", argc - 1);
        exit(EXIT_FAILURE);
    }

    printf("Welcome to wordle with %d letters!\n", word_len);

    wordle_t wordle = create_wordle(LANGUAGE_ENGLISH, word_len, "./words_alpha.txt");
    // wordle_t wordle = create_wordle(LANGUAGE_ENGLISH, word_len, "/usr/share/dict/words");
    if (wordle.word_length == 0) {
        fprintf(stderr, "Wordle database couldn't be generated!\n");
        exit(EXIT_FAILURE);
    }
    const int word_num = word_len < 7 ? 6 : 7;
    assert(word_num <= MAX_ROW_NUM);

    if (run_game_loop(&wordle, word_num, first_target) == false) {
        fprintf(stderr, "The game loop couldn't be run\n");
        exit(EXIT_FAILURE);
    }

    destroy_wordle(&wordle);

    return 0;
}
