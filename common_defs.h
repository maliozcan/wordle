#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H


#ifdef DEBUG
#define log(...)    printf(__VA_ARGS__)
#else
#define log(...)
#endif

// It must get more than one argument
#define TODO(fmt, ...) fprintf(stderr, "TODO: " fmt , __VA_ARGS__)

#define DEFAULT_WORD_LENGTH     (5)
#define MIN_WORD_LENGTH         (DEFAULT_WORD_LENGTH)
#define MAX_WORD_LENGTH         (8)
#define MAX_ROW_NUM             (8)


typedef enum {
    LANGUAGE_UNINITIALIZED,
    LANGUAGE_ENGLISH,
    NUM_OF_LANGUAGE,
} language_t;

typedef enum {
    NONE,
    EXIST,
    RIGHT_PLACE,
    NUM_OF_LETTER_POSITION,
} letter_position_type;

typedef enum {
    COLOR_DEFAULT,
    COLOR_GOLDEN,
    COLOR_GREEN,
    COLOR_WHITE,
    COLOR_RED,
} color_type;

#define default_color "\033[0m"
#define green_color "\033[92;100m"
#define golden_color "\033[93;100m"
#define red_color "\033[91;100m"

#ifndef TEST_BUILD
#define white_color "\033[97;100m"
#else
#define white_color ""
#endif

#endif // COMMON_DEFS_H
