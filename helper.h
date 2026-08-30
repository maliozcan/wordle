#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <wchar.h>
#include <ctype.h>
#include <wctype.h>

typedef int (*fptr_t) (int);
typedef wint_t (*wfptr_t) (wint_t);

static inline size_t print(FILE* f, const wchar_t* str, const size_t size, size_t* line_num)
{
    static wchar_t buffer[1024];
    assert(size < 1024L);
    memcpy(buffer, str, size * sizeof(wchar_t));
    buffer[size] = L'\0';
    fprintf(f, "%ls", buffer);
    for (size_t i = 0; i != size; ++i) {
        if (str[i] == L'\n') {
            ++(*line_num);
        }
    }
    return size;
}

static inline size_t print_newline(FILE* f, size_t* line_num)
{
    print(f, L"\n", 1, line_num);
    return 1;
}

static inline void transform_string(wchar_t* str, const size_t size, wfptr_t f)
{
    for (size_t i = 0; i != size; ++i) {
        str[i] = f(str[i]);
    }
}

static inline bool validate_word(const char* word, const size_t size, const int word_length)
{
    if (size < word_length) {
        return false;
    }
    bool in_word = false;
    int length_of_first_word = 0;
    for (size_t i = 0; i != size; ++i) {
        if (i == 0 && isalpha(word[i])) {
            length_of_first_word = 1;
            in_word = true;
        } else if (in_word == true && isalpha(word[i])) {
            ++length_of_first_word;
        } else if (in_word == true && isalpha(word[i] == 0)) {
            in_word = false;
            break;
        } else {
            break;
        }
    }
    for (size_t i = word_length; i != size; ++i) {
        if (isspace(word[i] == 0) ) {
            return false;
        }
    }
    if (length_of_first_word != word_length) {
        return false;
    }
    return true;
}

static inline bool validate_wc_word(const wchar_t* word, const size_t size, const int word_length)
{
    if (size < word_length) {
        return false;
    }
    bool in_word = false;
    int length_of_first_word = 0;
    for (size_t i = 0; i != size; ++i) {
        if (i == 0 && iswalpha(word[i])) {
            length_of_first_word = 1;
            in_word = true;
        } else if (in_word == true && iswalpha(word[i])) {
            ++length_of_first_word;
        } else if (in_word == true && iswalpha(word[i] == 0)) {
            in_word = false;
            break;
        } else {
            break;
        }
    }
    for (size_t i = word_length; i != size; ++i) {
        if (iswspace(word[i] == 0) ) {
            return false;
        }
    }
    if (length_of_first_word != word_length) {
        return false;
    }
    return true;
}


#endif // HELPER_H