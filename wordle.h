#ifndef WORDLE_H
#define WORDLE_H

#include "dynamic_array.h"

typedef enum {
    LANGUAGE_UNINITIALIZED,
    LANGUAGE_ENGLISH,
    NUM_OF_LANGUAGE,
} language_t;

typedef struct {
    language_t language;
    int word_length;
    dynamic_array_t words;
} wordle_t;

wordle_t create_wordle(language_t lang, int word_len, const char* filepath);
void destroy_wordle(wordle_t* wordle);
bool run_game_loop(wordle_t* wordle, int word_num);

#endif // WORDLE_H
