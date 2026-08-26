#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H

#if __STDC_VERSION__ < 201112L
    #error "This C version is not be supported"
#elif __STDC_VERSION__ < 202311L
    typedef char u8_t;
#else
    typedef char8_t u8_t;
#endif

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
    NONE,
    EXIST,
    RIGHT_PLACE,
    NUM_OF_LETTER_POSITION,
} letter_position_type;

#endif // COMMON_DEFS_H