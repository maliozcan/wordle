#ifndef HELPER_H
#define HELPER_H

typedef int (*fptr_t) (int);

static inline void transform_string(char* str, const size_t size, fptr_t f)
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


#endif // HELPER_H