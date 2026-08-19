#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#include "wordle.h"
#include "dynamic_array.h"
#include "helper.h"

dynamic_array_t _create_english_dictionary(const char* filepath, const int word_length);
void _print_dictionary(const wordle_t* wordle);

wordle_t create_wordle(language_t lang, int word_len, const char* filepath)
{
    // TODO: validate language
    wordle_t wordle = {0};
    wordle.words = _create_english_dictionary(filepath, word_len);
    if (is_dynamic_array_allocated(&wordle.words)) {
        wordle.language = lang;
        wordle.word_length = word_len;
        printf("Size of words which have %d letters is %zu\n", word_len, get_dynamic_array_size(&wordle.words));
        // _print_dictionary(&wordle);
    }
    return wordle;
}

void destroy_wordle(wordle_t* wordle)
{
    if (is_dynamic_array_allocated(&wordle->words)) {
        wordle->language = LANGUAGE_UNINITIALIZED;
        wordle->word_length = 0;
        free_dynamic_array(&wordle->words);
    }
}

void get_random_word(char word[], wordle_t* wordle)
{
    assert(wordle);
    srand(time(NULL));
    const size_t size = get_dynamic_array_size(&wordle->words);
    const size_t random_number = rand() % size;
    const char* src = get_dynamic_array_element(&wordle->words, random_number);
    memcpy(word, src, wordle->word_length);
}

dynamic_array_t _create_english_dictionary(const char* filepath, const int word_length)
{
    FILE* fd = fopen(filepath, "r");
    if (!fd) {
        perror("File opening failed");
        dynamic_array_t dynamic_array = {0};
        return dynamic_array;
    }

    char line[128] = {0}; // You may consider using dynamically allocated memory
    dynamic_array_t dynamic_word_list = create_dynamic_array(1000, word_length);
    int step_size = 0;
    while (fgets(line, sizeof(line), fd)) {
        const size_t line_size = strlen(line);
        assert(line_size != sizeof(line) - 1 && "Increase the line size");
        if (validate_word(line, line_size, word_length)) {
            append_dynamic_array(&dynamic_word_list, line);
        }
        step_size++;
    }
 
    fclose(fd);

    return dynamic_word_list;
}

void _print_dictionary(const wordle_t* wordle)
{
    if (wordle == NULL) {
        return;
    }
    const char* w0 = get_dynamic_array_element((dynamic_array_t*) &wordle->words, 444);
    char word[/*MAX_WORD_LENGTH*/8 + 1] = {0};
    memcpy(word, w0, wordle->word_length);
    word[wordle->word_length] = '\0';
    printf("first word %s\n", word);
}

#if 0
static bool has_space(const char* str, size_t size)
{
    bool result = false;
    for (size_t i = 0; i != size; ++i) {
        if (isspace(str[i])) {
            result = true;
            break;
        }
    }
    return result;
}

dynamic_array_t read_json(const char* path, const size_t size_hint)
{
    FILE* fd = fopen(path, "r");
    if (!fd) {
        perror("File opening failed");
        dynamic_array_t dynamic_word_list = {0};
        return dynamic_word_list;
    }

    char line[128] = {0}; // You may consider using dynamically allocated memory
    dynamic_array_t dynamic_word_list = create_dynamic_array(size_hint);
    int step_size = 0;
    while (fgets(line, sizeof(line), fd)) {
        const size_t line_size = strlen(line);
        assert(line_size != sizeof(line) - 1 && "Increase the line size");

        struct json_value_s* root = json_parse(line, line_size);
        assert(root->type == json_type_object);
        struct json_object_s* object = (struct json_object_s*)root->payload;

        struct json_object_element_s* e = object->start;
        for (int i = 0; i != object->length; ++i) {
            struct json_string_s* e_name = e->name;
            if (strcmp(e_name->string, "madde") == 0) {
                struct json_value_s* e_value = e->value;
                if (e_value->payload && e_value->type == json_type_string) {
                    struct json_string_s* e_value_string = e_value->payload;
                    const char* word = e_value_string->string;
                    const size_t len = strlen(word);
                    if (len == 5 && has_space(word, len) == false) {
                        printf("step_size %d: element name: %s  value: %s\n", step_size, e_name->string, e_value_string->string);
                    }
                }
            }
            e = e->next;
        } 

        // append_dynamic_array(&dynamic_json_list, line, line_size);
        step_size++;
    }
 
    fclose(fd);
 
    return dynamic_word_list;
}
#endif