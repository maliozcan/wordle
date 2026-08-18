#include <string.h>

#include "utest.h"

#include "dynamic_array.h"

UTEST_MAIN()

UTEST(dynamic_array, char_array)
{
    size_t capacity = 5;
    dynamic_array_t dynamic_array = create_dynamic_array(capacity, sizeof(char));
    ASSERT_TRUE(is_dynamic_array_allocated(&dynamic_array));
    ASSERT_EQ(get_dynamic_array_capacity(&dynamic_array), capacity);
    ASSERT_EQ(get_dynamic_array_size(&dynamic_array), 0);
    
    const char* str = "ABCDE";
    append_bulk_dynamic_array(&dynamic_array, str, strlen(str));
    for (size_t i = 0; i != 5; ++i) {
        ASSERT_EQ(*((char*)get_dynamic_array_element(&dynamic_array, i)), str[i]);
    }
    ASSERT_EQ(get_dynamic_array_size(&dynamic_array), 5);
    ASSERT_EQ(get_dynamic_array_capacity(&dynamic_array), capacity);

    append_dynamic_array(&dynamic_array, "F");
    ASSERT_EQ(*((char*)get_dynamic_array_element(&dynamic_array, 5)), 'F');
    ASSERT_EQ(get_dynamic_array_size(&dynamic_array), 6);
    capacity *= 2;
    ASSERT_EQ(get_dynamic_array_capacity(&dynamic_array), capacity);

    // Test the first elements after resizing
    for (size_t i = 0; i != 5; ++i) {
        ASSERT_EQ(*((char*)get_dynamic_array_element(&dynamic_array, i)), str[i]);
    }

    const size_t new_capacity = capacity * 8;
    ASSERT_TRUE(resize_dynamic_array(&dynamic_array, new_capacity));
    ASSERT_EQ(get_dynamic_array_size(&dynamic_array), 6);
    ASSERT_EQ(get_dynamic_array_capacity(&dynamic_array), new_capacity);

    // Test the first elements after resizing
    for (size_t i = 0; i != 5; ++i) {
        ASSERT_EQ(*((char*)get_dynamic_array_element(&dynamic_array, i)), str[i]);
    }

    free_dynamic_array(&dynamic_array);
}

UTEST(dynamic_array, int_array)
{
    size_t capacity = 5;
    dynamic_array_t dynamic_array = create_dynamic_array(capacity, sizeof(int));
    ASSERT_TRUE(is_dynamic_array_allocated(&dynamic_array));
    ASSERT_EQ(get_dynamic_array_capacity(&dynamic_array), capacity);
    ASSERT_EQ(get_dynamic_array_size(&dynamic_array), 0);

    int a[3] = {-42, 0, 42};
    append_bulk_dynamic_array(&dynamic_array, &a, sizeof(a)/sizeof(a[0]));
    ASSERT_EQ(*((int*)get_dynamic_array_element(&dynamic_array, 0)), a[0]);
    ASSERT_EQ(*((int*)get_dynamic_array_element(&dynamic_array, 1)), a[1]);
    ASSERT_EQ(*((int*)get_dynamic_array_element(&dynamic_array, 2)), a[2]);
    int b = 100;
    append_dynamic_array(&dynamic_array, &b);
    b = 225;
    append_dynamic_array(&dynamic_array, &b);
    ASSERT_EQ(*((int*)get_dynamic_array_element(&dynamic_array, 3)), 100);
    ASSERT_EQ(*((int*)get_dynamic_array_element(&dynamic_array, 4)), 225);
    ASSERT_EQ(get_dynamic_array_size(&dynamic_array), 5);
    ASSERT_EQ(get_dynamic_array_capacity(&dynamic_array), capacity);
    b = -1;
    append_dynamic_array(&dynamic_array, &b);
    capacity *= 2;
    ASSERT_EQ(*((int*)get_dynamic_array_element(&dynamic_array, 5)), -1);
    ASSERT_EQ(get_dynamic_array_size(&dynamic_array), 6);
    ASSERT_EQ(get_dynamic_array_capacity(&dynamic_array), capacity);

    // Test the first elements after resizing
    ASSERT_EQ(*((int*)get_dynamic_array_element(&dynamic_array, 0)), a[0]);
    ASSERT_EQ(*((int*)get_dynamic_array_element(&dynamic_array, 1)), a[1]);
    ASSERT_EQ(*((int*)get_dynamic_array_element(&dynamic_array, 2)), a[2]);

    const size_t new_capacity = capacity * 25;
    ASSERT_TRUE(resize_dynamic_array(&dynamic_array, new_capacity));
    ASSERT_EQ(get_dynamic_array_size(&dynamic_array), 6);
    ASSERT_EQ(get_dynamic_array_capacity(&dynamic_array), new_capacity);

    // Test the first elements after resizing
    ASSERT_EQ(*((int*)get_dynamic_array_element(&dynamic_array, 0)), a[0]);
    ASSERT_EQ(*((int*)get_dynamic_array_element(&dynamic_array, 1)), a[1]);
    ASSERT_EQ(*((int*)get_dynamic_array_element(&dynamic_array, 2)), a[2]);

    free_dynamic_array(&dynamic_array);
}

UTEST(dynamic_array, string_array)
{
    size_t capacity = 3;
    size_t string_length = 5;
    dynamic_array_t dynamic_array = create_dynamic_array(capacity, string_length);
    ASSERT_TRUE(is_dynamic_array_allocated(&dynamic_array));
    ASSERT_EQ(get_dynamic_array_capacity(&dynamic_array), capacity);
    ASSERT_EQ(get_dynamic_array_size(&dynamic_array), 0);

    const char* str = "ABCDEfghijKLMNP";
    append_bulk_dynamic_array(&dynamic_array, str, 3);
    ASSERT_EQ(strncmp((const char*)get_dynamic_array_element(&dynamic_array, 0), "ABCDE", string_length), 0);
    ASSERT_EQ(strncmp((const char*)get_dynamic_array_element(&dynamic_array, 1), "fghij", string_length), 0);
    ASSERT_EQ(strncmp((const char*)get_dynamic_array_element(&dynamic_array, 2), "KLMNP", string_length), 0);
    ASSERT_EQ(get_dynamic_array_size(&dynamic_array), 3);
    ASSERT_EQ(get_dynamic_array_capacity(&dynamic_array), capacity);

    str = "Table";
    append_dynamic_array(&dynamic_array, str);
    ASSERT_EQ(strncmp((const char*)get_dynamic_array_element(&dynamic_array, 3), "Table", string_length), 0);
    ASSERT_EQ(get_dynamic_array_size(&dynamic_array), 4);
    capacity *= 2;
    ASSERT_EQ(get_dynamic_array_capacity(&dynamic_array), capacity);

    // Test the first elements after resizing
    ASSERT_EQ(strncmp((const char*)get_dynamic_array_element(&dynamic_array, 0), "ABCDE", string_length), 0);
    ASSERT_EQ(strncmp((const char*)get_dynamic_array_element(&dynamic_array, 1), "fghij", string_length), 0);
    ASSERT_EQ(strncmp((const char*)get_dynamic_array_element(&dynamic_array, 2), "KLMNP", string_length), 0);

    const size_t new_capacity = capacity * 15;
    ASSERT_TRUE(resize_dynamic_array(&dynamic_array, new_capacity));
    ASSERT_EQ(get_dynamic_array_size(&dynamic_array), 4);
    ASSERT_EQ(get_dynamic_array_capacity(&dynamic_array), new_capacity);

    // Test the first elements after resizing
    ASSERT_EQ(strncmp((const char*)get_dynamic_array_element(&dynamic_array, 0), "ABCDE", string_length), 0);
    ASSERT_EQ(strncmp((const char*)get_dynamic_array_element(&dynamic_array, 1), "fghij", string_length), 0);
    ASSERT_EQ(strncmp((const char*)get_dynamic_array_element(&dynamic_array, 2), "KLMNP", string_length), 0);

    free_dynamic_array(&dynamic_array);
}
