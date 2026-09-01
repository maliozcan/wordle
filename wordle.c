#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <wchar.h>

#include "wordle.h"
#include "layout.h"
#include "common_defs.h"
#include "helper.h"
#include "dynamic_array.h"


// #define DICTONARY_OPTIMIZATION_FOR_MEMORY
// #define DEBUG_WORDLE

dynamic_array_t _create_english_dictionary(const char* filepath, const int word_length, const bool use_wide_char);
void _print_dictionary(const wordle_t* wordle);
static void _empty_stdin(wchar_t buf[], const size_t buf_size);
static bool _ask_user_if_they_want_to_continue(layout_handler_t layout_handler, int* word_order);
static void _get_random_word(wchar_t word[], wordle_t* wordle);
static bool _is_word_in_dictionary(wchar_t word[], wordle_t* wordle);
static void _clear_stdout(const size_t line_num);
static void _print_info(const wchar_t* str, size_t* line_num);

#ifndef TEST_BUILD
static void find_position(const wchar_t word[MAX_WORD_LENGTH], const wchar_t target[MAX_WORD_LENGTH + 1], const int word_len, letter_position_type position[MAX_WORD_LENGTH]);
#endif

wordle_t create_wordle(const language_t lang, const int word_len, const char* filepath)
{
    wordle_t wordle = {0};
    if (lang == LANGUAGE_UNINITIALIZED || lang >= NUM_OF_LANGUAGE) {
        return wordle;
    }

    if (lang == LANGUAGE_ENGLISH) {
        const char* english_code = "en_US.UTF-8";
        const char* l;
        if ((l = setlocale(LC_ALL, english_code)) == NULL) {
            fprintf(stderr, "Couldn't set locale as %s\n", english_code);
            exit(1);
        }
#ifdef DEBUG_WORDLE
        printf("locale: %s\n", l);
#endif
    } else {
        fprintf(stderr, "Doesn't support the language [%d]\n", lang);
        exit(1);
    }

#ifdef DICTONARY_OPTIMIZATION_FOR_MEMORY
    const bool use_wide_char = (lang != LANGUAGE_ENGLISH); // Note that ASCII includes all letter in English.
#else
    const bool use_wide_char = true;
#endif
    wordle.words = _create_english_dictionary(filepath, word_len, use_wide_char);
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

bool run_game_loop(wordle_t* wordle, int word_num)
{
    if (wordle == NULL || is_dynamic_array_allocated(&wordle->words) == false) {
        return false;
    }

    const int word_len = wordle->word_length;
    if (word_len < MIN_WORD_LENGTH || word_len > MAX_WORD_LENGTH) {
        return false;
    }

    layout_handler_t layout_handler = initialize_layout(word_len, word_num);
    if (NULL == layout_handler) {
        fprintf(stderr, "The layout didn't initialized\n");
        return false;
    }

    size_t line_num = 0;

    letter_position_type position[MAX_WORD_LENGTH] = {0};
    for (int i = 0; i != MAX_WORD_LENGTH; ++i) {
        position[i] = -1;
    }
    wchar_t target[MAX_WORD_LENGTH + 1] = {0};
    _get_random_word(target, wordle);
    target[word_len] = '\0';
    wchar_t input[32] = {0};
    wchar_t word[MAX_WORD_LENGTH + 1] = {0};
    int word_order = 0;
    bool quit = false;
    bool first_time = true;
    while (false == quit) {
        if (false == first_time) {
            _clear_stdout(line_num);
            line_num = 0;
        }
        first_time = false;
        draw_layout(layout_handler, &line_num);
        _print_info(L"> ", &line_num); ++line_num;
        if (fgetws(input, sizeof(input) / sizeof(wchar_t), stdin) == NULL) {
            printf("\n");
            break;
        }
        const int str_size = wcslen(input);
        if (validate_wc_word(input, str_size, word_len) && _is_word_in_dictionary(input, wordle)) {
            memcpy(word, input, word_len * sizeof(wchar_t));
            transform_string(word, word_len, towlower);
            find_position(word, target, word_len, position);
            add_word(layout_handler, word, word_order, position);
            ++word_order;
            if (wcsncmp(word, target, word_len) == 0) {
                _clear_stdout(line_num);
                line_num = 0;
                draw_layout(layout_handler, &line_num);
                _print_info(L"You found the word. If you want to continue, type [yes]: ", &line_num); ++line_num;
                quit = _ask_user_if_they_want_to_continue(layout_handler, &word_order);
                if (false == quit) {
                    _get_random_word(target, wordle);
                    target[word_len] = L'\0';
                }
            } else if (word_num == word_order) {
                _clear_stdout(line_num);
                line_num = 0;
                draw_layout(layout_handler, &line_num);
                transform_string(target, word_len, towupper);
                printf("You didn't find the word \"%ls\". If you want to continue, type [yes]: ", target); ++line_num;
                quit = _ask_user_if_they_want_to_continue(layout_handler, &word_order);
                if (false == quit) {
                    _get_random_word(target, wordle);
                    target[word_len] = L'\0';
                }
            }
        } else {
            _print_info(L"Word is not valid!\n", &line_num);
            _empty_stdin(input, sizeof(input) / sizeof(wchar_t));
        }
        word[0] = '\0';
    }

    destroy_layout(layout_handler);
    return true;
}

static void _get_random_word(wchar_t word[], wordle_t* wordle)
{
    assert(wordle);
    srand(time(NULL));
    const size_t size = get_dynamic_array_size(&wordle->words);
    const size_t random_number = rand() % size;
    assert(wordle->language == LANGUAGE_ENGLISH);
#ifdef DICTONARY_OPTIMIZATION_FOR_MEMORY
    if (wordle->language == LANGUAGE_ENGLISH) {
        const char* src = get_dynamic_array_element(&wordle->words, random_number);
        for (int i = 0; i != wordle->word_length; ++i) {
            word[i] = src[i];
        }
    } else
#endif
    {
        const wchar_t* src = get_dynamic_array_element(&wordle->words, random_number);
        wcsncpy(word, src, wordle->word_length);
    }
}

dynamic_array_t _create_english_dictionary(const char* filepath, const int word_length, const bool use_wide_char)
{
    FILE* fd = fopen(filepath, "r");
    if (!fd) {
        perror("File opening failed");
        dynamic_array_t dynamic_array = {0};
        return dynamic_array;
    }

    const size_t char_size_in_dict = use_wide_char ? sizeof(wchar_t) : sizeof(char);
    dynamic_array_t dynamic_word_list = create_dynamic_array(1000, word_length * char_size_in_dict);

    if (use_wide_char == false) {
        char line[128] = {0}; // You may consider using dynamically allocated memory
        while (fgets(line, sizeof(line), fd)) {
            const size_t line_size = strlen(line);
            assert(line_size != sizeof(line) - 1 && "Increase the line size");
            if (validate_word(line, line_size, word_length)) {
                append_dynamic_array(&dynamic_word_list, line);
            }
        }
    } else {
        wchar_t line[128] = {0}; // You may consider using dynamically allocated memory
        while (fgetws(line, sizeof(line) / sizeof(wchar_t), fd)) {
            const size_t line_size = wcslen(line);
            assert(line_size != (sizeof(line) / sizeof(wchar_t) - 1) && "Increase the line size");
            if (validate_wc_word(line, line_size, word_length)) {
                append_dynamic_array(&dynamic_word_list, line);
            }
        }
    }
 
    fclose(fd);

    return dynamic_word_list;
}

void _print_dictionary(const wordle_t* wordle)
{
    if (wordle == NULL) {
        return;
    }
    const char* w0 = get_dynamic_array_element((dynamic_array_t*) &wordle->words, 0);
    char word[MAX_WORD_LENGTH + 1] = {0};
    memcpy(word, w0, wordle->word_length);
    word[wordle->word_length] = '\0';
    printf("first word %s\n", word);
}

static void _empty_stdin(wchar_t buf[], const size_t buf_size)
{
    if (buf == NULL) {
        return;
    }
    while (wcslen(buf) == buf_size - 1) {
        fgetws(buf, buf_size, stdin);
    }
    buf[0] = L'\0';
}

static bool _ask_user_if_they_want_to_continue(layout_handler_t layout_handler, int* word_order)
{
    wchar_t input[16];
    if (fgetws(input, sizeof(input), stdin) == NULL) {
        printf("\n");
        return true;
    }
    bool quit = false;
    transform_string(input, wcslen(input), towlower);
    if (wcsncmp(input, L"yes", 3) == 0) {
        clear_layout(layout_handler);
        *word_order = 0;
        _empty_stdin(input, sizeof(input) / sizeof(wchar_t));
    } else {
        quit = true;
    }
    return quit;
}

static bool _is_word_in_dictionary(wchar_t word[], wordle_t* wordle)
{
#ifdef DICTONARY_OPTIMIZATION_FOR_MEMORY
    if (wordle->language == LANGUAGE_ENGLISH) {
        char buf[MAX_WORD_LENGTH] = {0};
        for (int i = 0; i != wordle->word_length; ++i) {
            // buf[i] = ((int)word[i]) & 0X000000FF; // Can it be written as portable?
            buf[i] = (char)word[i]; // Can it be written as portable?
        }
        return find_element_in_dynamic_array(&wordle->words, buf);
    } else
#endif
    {
        return find_element_in_dynamic_array(&wordle->words, word);
    }
}

static void _clear_stdout(const size_t line_num)
{
#ifndef DEBUG_WORDLE
    for (size_t i = 0; i != line_num; ++i) {
        printf("\033[A\033[2K");
    }
    fflush(stdout);
#endif
}

static void _print_info(const wchar_t* str, size_t* line_num)
{
    print(stdout, str, wcslen(str), line_num);
}

static size_t _count_character(const wchar_t str[], const size_t size, const wchar_t c)
{
    size_t total = 0;
    for (size_t i = 0; i != size; ++i) {
        if (str[i] == c) {
            ++total;
        }
    }
    return total;
}

static bool _check_upper_case(const wchar_t word[MAX_WORD_LENGTH], const int word_len)
{
    bool result = true;
    for (int i = 0; i != word_len; ++i) {
        if (iswupper(word[i]) == 0) {
            result = false;
            break;
        }
    }
    return result;
}

void find_position(const wchar_t word[MAX_WORD_LENGTH], const wchar_t target[MAX_WORD_LENGTH + 1], const int word_len, letter_position_type position[MAX_WORD_LENGTH])
{
    assert(_check_upper_case(word, word_len));
    assert(_check_upper_case(target, word_len));

    for (int i = 0; i != word_len; ++i) {
        if (word[i] == target[i]) {
            position[i] = RIGHT_PLACE;
        } else if (wcschr(target, word[i])) {
            position[i] = EXIST;
        } else {
            position[i] = NONE;
        }
    }
    // When the target is DUMMY, the first letter in the word MUMMY shouldn't be in EXIST state.
    for (int i = 0; i != word_len; ++i) {
        if (position[i] != RIGHT_PLACE) {
            continue;
        }
        for (int j = 0; j != word_len; ++j) {
            if (j == i || target[j] == target[i]) {
                continue;
            }
            if (target[i] == word[j]) {
                position[j] = NONE;
            }
        }
    }

    // When the target is "DUMMY", an imaginary word "CADDD" must have EXIST state for the first D,
    // the remainings must be NONE since there is one 'D' letter in "DUMMY".
    for (int i = 0; i != word_len; ++i) {
        if (position[i] != EXIST) {
            continue;
        }
        size_t count_of_existing = _count_character(target, word_len, word[i]);
        for (int j = 0; j != word_len; ++j) {
            if (word[i] == word[j]) {
                if (count_of_existing > 0) {
                    --count_of_existing;
                } else {
                    position[j] = NONE;
                }
            }
        }
    }
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
