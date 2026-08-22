#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "wordle.h"
#include "common_defs.h"

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

    wordle_t wordle = create_wordle(LANGUAGE_ENGLISH, word_len, "./words_alpha.txt");
    // wordle_t wordle = create_wordle(LANGUAGE_ENGLISH, word_len, "/usr/share/dict/words");
    if (wordle.word_length == 0) {
        fprintf(stderr, "Wordle database couldn't be generated!\n");
        exit(EXIT_FAILURE);
    }
    const int word_num = word_len < 7 ? 6 : 7;

    if (run_game_loop(&wordle, word_num) == false) {
        fprintf(stderr, "The game loop couldn't be run\n");
        exit(EXIT_FAILURE);
    }

    destroy_wordle(&wordle);

    return 0;
}
