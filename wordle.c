#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#include "wordle.h"
#include "layout.h"
#include "common_defs.h"
#include "helper.h"
#include "dynamic_array.h"

dynamic_array_t _create_english_dictionary(const char* filepath, const int word_length);
void _print_dictionary(const wordle_t* wordle);
static void _empty_stdin(char buf[], const size_t buf_size);
static bool _ask_user_if_they_want_to_continue(layout_handler_t layout_handler, int* word_order);
static void _get_random_word(char word[], wordle_t* wordle);
static bool _is_word_in_dictionary(char word[], wordle_t* wordle);
static void _clear_stdout(const size_t line_num);
static void _print_info(const char* str, size_t* line_num);
static void _find_position(const char word[MAX_WORD_LENGTH], const char target[MAX_WORD_LENGTH + 1], const int word_len, letter_position_type position[MAX_WORD_LENGTH]);

wordle_t create_wordle(const language_t lang, const int word_len, const char* filepath)
{
    wordle_t wordle = {0};
    if (lang == LANGUAGE_UNINITIALIZED || lang >= NUM_OF_LANGUAGE) {
        return wordle;
    }
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
    char target[MAX_WORD_LENGTH + 1] = {0};
    _get_random_word(target, wordle);
    target[word_len] = '\0';
    char input[32] = {0};
    char word[MAX_WORD_LENGTH + 1] = {0};
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
        _print_info("> ", &line_num); ++line_num;
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break;
        }
        const int str_size = strlen(input);
        if (validate_word(input, str_size, word_len) && _is_word_in_dictionary(input, wordle)) {
            memcpy(word, input, word_len);
            transform_string(word, word_len, tolower);
            _find_position(word, target, word_len, position);
            add_word(layout_handler, word, word_order, position);
            ++word_order;
            if (strncmp(word, target, word_len) == 0) {
                _clear_stdout(line_num);
                line_num = 0;
                draw_layout(layout_handler, &line_num);
                _print_info("You found the word. If you want to continue, type [yes]: ", &line_num); ++line_num;
                quit = _ask_user_if_they_want_to_continue(layout_handler, &word_order);
                if (false == quit) {
                    _get_random_word(target, wordle);
                    target[word_len] = '\0';
                }
            }
            if (word_num == word_order) {
                _clear_stdout(line_num);
                line_num = 0;
                draw_layout(layout_handler, &line_num);
                transform_string(target, word_len, toupper);
                printf("You didn't find the word \"%s\". If you want to continue, type [yes]: ", target); ++line_num;
                quit = _ask_user_if_they_want_to_continue(layout_handler, &word_order);
                if (false == quit) {
                    _get_random_word(target, wordle);
                    target[word_len] = '\0';
                }
            }
        } else {
            _print_info("Word is not valid!\n", &line_num);
            _empty_stdin(input, sizeof(input));
        }
        word[0] = '\0';
    }

    destroy_layout(layout_handler);
    return true;
}

static void _get_random_word(char word[], wordle_t* wordle)
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
    char word[MAX_WORD_LENGTH + 1] = {0};
    memcpy(word, w0, wordle->word_length);
    word[wordle->word_length] = '\0';
    printf("first word %s\n", word);
}

static void _empty_stdin(char buf[], const size_t buf_size)
{
    if (buf == NULL) {
        return;
    }
    while (strlen(buf) == buf_size - 1) {
        fgets(buf, buf_size, stdin);
    }
    buf[0] = '\0';
}

static bool _ask_user_if_they_want_to_continue(layout_handler_t layout_handler, int* word_order)
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
        _empty_stdin(input, sizeof(input));
    } else {
        quit = true;
    }
    return quit;
}

static bool _is_word_in_dictionary(char word[], wordle_t* wordle)
{
    return find_element_in_dynamic_array(&wordle->words, word);
}

static void _clear_stdout(const size_t line_num)
{
    for (size_t i = 0; i != line_num; ++i) {
        printf("\033[A\033[2K");
    }
    fflush(stdout);
}

static void _print_info(const char* str, size_t* line_num)
{
    print(stdout, str, strlen(str), line_num);
}

static size_t _count_character(const char str[], const size_t size, const int c)
{
    size_t total = 0;
    for (size_t i = 0; i != size; ++i) {
        if (str[i] == c) {
            ++total;
        }
    }
    return total;
}

static void _find_position(const char word[MAX_WORD_LENGTH], const char target[MAX_WORD_LENGTH + 1], const int word_len, letter_position_type position[MAX_WORD_LENGTH])
{
    for (int i = 0; i != word_len; ++i) {
        if (word[i] == target[i]) {
            position[i] = RIGHT_PLACE;
        } else if (strchr(target, word[i])) {
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