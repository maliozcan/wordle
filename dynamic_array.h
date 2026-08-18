#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

/*
    This is a single-header library to allocate array/list.
    Note: There is no macro to separate interface and implementation.
          Every function is 'static inline' currently.
*/

typedef struct {
    void* buf;
    size_t capacity;
    size_t used_size;
    size_t element_size; // in byte
} dynamic_array_t;

// Interface

static inline dynamic_array_t create_dynamic_array(const size_t capacity, const size_t element_size);
static inline bool reallocate_dynamic_array(dynamic_array_t* dynamic_array, const size_t capacity, const size_t element_size);
static inline bool resize_dynamic_array(dynamic_array_t* dynamic_array, const size_t new_capacity);
static inline bool is_dynamic_array_allocated(dynamic_array_t* dynamic_array);
static inline void free_dynamic_array(dynamic_array_t* dynamic_array);
static inline void* get_dynamic_array_element(dynamic_array_t* dynamic_array, size_t index);
static inline bool append_bulk_dynamic_array(dynamic_array_t* dynamic_array, const void* src, size_t size);
static inline bool append_dynamic_array(dynamic_array_t* dynamic_array, const void* src);
static inline size_t get_dynamic_array_size(dynamic_array_t* dynamic_array);
static inline size_t get_dynamic_array_capacity(dynamic_array_t* dynamic_array);


// Implementation

static inline bool _allocate_dynamic_array_buffer(dynamic_array_t* dynamic_array, const size_t capacity, const size_t element_size)
{
    dynamic_array->buf = malloc(element_size * capacity);
    if (dynamic_array->buf) {
        dynamic_array->capacity = capacity;
        dynamic_array->element_size = element_size;
        return true;
    }
    return false;
}

static inline dynamic_array_t create_dynamic_array(const size_t capacity, const size_t element_size)
{
    dynamic_array_t dynamic_array = {0};
    _allocate_dynamic_array_buffer(&dynamic_array, capacity, element_size);
    return dynamic_array;
}

static inline bool reallocate_dynamic_array(dynamic_array_t* dynamic_array, const size_t capacity, const size_t element_size)
{
    assert(dynamic_array);
    if (dynamic_array->buf) {
        return false;
    }
    return _allocate_dynamic_array_buffer(dynamic_array, capacity, element_size);
}

static inline bool _do_resize_dynamic_array(dynamic_array_t* dynamic_array, size_t new_capacity, size_t element_size)
{
    void* buf = realloc(dynamic_array->buf, new_capacity * element_size);
    if (buf == NULL) {
        return false;
    }
    dynamic_array->buf = buf;
    dynamic_array->capacity = new_capacity;
    return true;
}

static inline bool resize_dynamic_array(dynamic_array_t* dynamic_array, const size_t new_capacity)
{
    assert(dynamic_array);
    if (dynamic_array->capacity > new_capacity) {
        return false;
    }
    return _do_resize_dynamic_array(dynamic_array, new_capacity, dynamic_array->element_size);
}

static inline bool is_dynamic_array_allocated(dynamic_array_t* dynamic_array)
{
    assert(dynamic_array);
    return dynamic_array->buf;
}

static inline void free_dynamic_array(dynamic_array_t* dynamic_array)
{
    assert(dynamic_array);
    if (dynamic_array->buf)
    {
        dynamic_array->capacity = 0;
        dynamic_array->used_size = 0;
        dynamic_array->element_size = 0;
        free(dynamic_array->buf);
    }
}

static inline bool _append_bulk_dynamic_array(dynamic_array_t* dynamic_array, const void* src, size_t size, size_t element_size)
{
    assert(dynamic_array);
    if (dynamic_array->buf == NULL) {
        fprintf(stderr, "[append_dynamic_array] the buffer in dynamic_array is null\n");
        return false;
    }
    if (src == NULL) {
        fprintf(stderr, "[append_dynamic_array] src is null\n");
        return false;
    }

    const size_t size_needed = dynamic_array->used_size + size;
    if (size_needed > dynamic_array->capacity) {
        const size_t new_capacity = (dynamic_array->capacity * 2) > size_needed ?
                                    (dynamic_array->capacity * 2) : size_needed;
        if (_do_resize_dynamic_array(dynamic_array, new_capacity, element_size) == false) {
            return false;
        }
    }

    memcpy((char*)dynamic_array->buf + (dynamic_array->used_size * element_size), src, size * element_size);
    dynamic_array->used_size += size;
    return true;
}

static inline void* get_dynamic_array_element(dynamic_array_t* dynamic_array, size_t index)
{
    assert(dynamic_array);
    return (char*)dynamic_array->buf + index * dynamic_array->element_size;
}

static inline bool append_bulk_dynamic_array(dynamic_array_t* dynamic_array, const void* src, size_t size)
{
    assert(dynamic_array);
    return _append_bulk_dynamic_array(dynamic_array, src, size, dynamic_array->element_size);
}

static inline bool append_dynamic_array(dynamic_array_t* dynamic_array, const void* src)
{
    assert(dynamic_array);
    return _append_bulk_dynamic_array(dynamic_array, src, 1, dynamic_array->element_size);
}

static inline size_t get_dynamic_array_size(dynamic_array_t* dynamic_array)
{
    assert(dynamic_array);
    return dynamic_array->used_size;
}

static inline size_t get_dynamic_array_capacity(dynamic_array_t* dynamic_array)
{
    assert(dynamic_array);
    return dynamic_array->capacity;
}

#endif // DYNAMIC_ARRAY_H