#include <stdio.h>
#include <assert.h>

#include "utest.h"

#include "wordle.h"
#include "common_defs.h"
#include "dynamic_array.h"


static const char* letter_position_to_str[NUM_OF_LETTER_POSITION] = {
    "NONE",
    "EXIST",
    "RIGHT_PLACE"
};

_Static_assert(NONE == 0, "");
_Static_assert(EXIST == 1, "");
_Static_assert(RIGHT_PLACE == 2, "");

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
    printf("target: %s, word: %s: position[", target, word);    \
    for (int i = 0; i != word_len; ++i) {                       \
        printf("%s%s", letter_position_to_str[position[i]],     \
            (i == word_len - 1) ? "" : ", ");                   \
    }                                                           \
    printf("]\n");                                              \
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

UTEST(wordle_position, example_1) {
    // When the target is DUMMY, the first letter in the word MUMMY shouldn't be in EXIST state.
    const int word_len = 5;
    const char* target = "DUMMY";
    const char* word = "MUMMY";
    letter_position_type expected[MAX_WORD_LENGTH] = {NONE, RIGHT_PLACE, RIGHT_PLACE, RIGHT_PLACE, RIGHT_PLACE};
    TEST_POSITION(word_len, target, word, expected);
}

UTEST(wordle_position, example_2) {
    // When the target is "DUMMY", an imaginary word "CADDD" must have EXIST state for the first D,
    // the remainings must be NONE since there is one 'D' letter in "DUMMY".
    const int word_len = 5;
    const char* target = "DUMMY";
    const char* word = "CADDD";
    letter_position_type expected[MAX_WORD_LENGTH] = {NONE, NONE, EXIST, NONE, NONE};
    TEST_POSITION(word_len, target, word, expected);
}

UTEST(wordle_position, example_3) {
    // When the target is "ROOKY", the both O in "BLOOD" must have RIGHT_PLACE state.
    const int word_len = 5;
    const char* target = "ROOKY";
    const char* word = "BLOOD";
    letter_position_type expected[MAX_WORD_LENGTH] = {NONE, NONE, RIGHT_PLACE, EXIST, NONE};
    TEST_POSITION(word_len, target, word, expected);
}
