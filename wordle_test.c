#include <stdio.h>
#include <assert.h>

#include "utest.h"

#include "wordle.h"
#include "common_defs.h"
#include "dynamic_array.h"

static void set_word(const int word_len, const char* src, wchar_t dest[MAX_WORD_LENGTH])
{
    assert(src != NULL);
    for(int i = 0; i != word_len && src[i] != '\0'; ++i) {
        dest[i] = src[i];
    }

}

// test_position(const int word_len, const char* target, const char* word, letter_position_type expected[MAX_WORD_LENGTH])
#define TEST_POSITION(word_len, target, word, expected)         \
{                                                               \
    letter_position_type position[MAX_WORD_LENGTH] = {0};       \
    wchar_t target_wc[MAX_WORD_LENGTH + 1];                     \
    set_word(word_len, target, target_wc);                      \
    wchar_t word_wc[MAX_WORD_LENGTH];                           \
    set_word(word_len, word, word_wc);                          \
    find_position(word_wc, target_wc, word_len, position);      \
                                                                \
    for (int i = 0; i != word_len; ++i) {                       \
        ASSERT_EQ(position[i], expected[i]);                    \
    }                                                           \
}

UTEST_MAIN()

UTEST(wordle_initializtion, all_num_of_letters) {
    for (int word_len = MIN_WORD_LENGTH; word_len <= MAX_WORD_LENGTH; ++word_len) {
        printf("Creating wordle for %d letters\n", word_len);
        wordle_t wordle = create_wordle(LANGUAGE_ENGLISH, word_len, "./words_alpha.txt");
    
        ASSERT_EQ(wordle.word_length, word_len);
        ASSERT_EQ(wordle.language, LANGUAGE_ENGLISH);
        ASSERT_TRUE(is_dynamic_array_allocated(&wordle.words));
    
        destroy_wordle(&wordle);
    
        ASSERT_EQ(wordle.word_length, 0);
        ASSERT_EQ(wordle.language, LANGUAGE_UNINITIALIZED);
        ASSERT_FALSE(is_dynamic_array_allocated(&wordle.words));
    }
}

UTEST(wordle_position, no_match) {
    const int word_len = 5;
    const char* target = "ABCDE";
    const char* word = "FGHIJ";
    letter_position_type expected[MAX_WORD_LENGTH] = {NONE, NONE, NONE, NONE, NONE};
    TEST_POSITION(word_len, target, word, expected);
}

UTEST(wordle_position, all_match) {
    const int word_len = 5;
    const char* target = "ABCDE";
    const char* word = "ABCDE";
    letter_position_type expected[MAX_WORD_LENGTH] = {RIGHT_PLACE, RIGHT_PLACE, RIGHT_PLACE, RIGHT_PLACE, RIGHT_PLACE};
    TEST_POSITION(word_len, target, word, expected);
}
