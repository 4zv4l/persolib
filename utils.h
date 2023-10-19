#ifndef UTILS
#define UTILS

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include "types.h"

// foreach for array (known size)
#define foreach(var, arr)                                                   \
    for(                                                                    \
            usize index = 0, done = 1, length = sizeof(arr)/sizeof(arr[0]); \
            index < length;                                                 \
            done = 1, index++                                               \
    )                                                                       \
    for(typeof(arr[0]) var = arr[index]; done; done = 0)

// foreach for pointer (null terminated)
#define pforeach(var, ptr)                                                  \
    for(                                                                    \
            usize index = 0, done = 1;                                      \
            ptr[index] != 0;                                                \
            done = 1, index++                                               \
    )                                                                       \
    for(typeof(ptr[0]) var = ptr[index]; done; done = 0)

// foreach for pointer (known length)
#define plforeach(var, ptr, len)                                            \
    for(                                                                    \
            usize index = 0, done = 1;                                      \
            index < len;                                                    \
            done = 1, index++                                               \
    )                                                                       \
    for(typeof(ptr[0]) var = ptr[index]; done; done = 0)

#define ARRAY_LEN(arr) sizeof((arr))/sizeof((*arr))

/*
 * duplicate data structure
 * return a pointer to that data
 * same as malloc + memcopy
 *
 * return 0 if malloc failed
 */
void *dupcopy(void *data, usize size);

/*
 * return the 'size' of a ptr
 * loop until finding a 0 value
 * the return value does not count the 0
 */
usize ptr_len(void *data, usize size);

/*
 * remove last newline from string
 */
string chomp(char str[static 1]);

/*
 * return the index of c in str
 * or 0 if not found
 */
usize index_of(char c, char str[static 1]);

/*
 * convert str to hexa
 * store it in hex
 * hex should be at least 2x size of str
 */
void bytes_to_hex(char hex[static 1], const char str[static 1], int strlen);

/*
 * convert hex to bytes
 * store it in str
 * str should be at least 1/2 size of hex
 */
void hex_to_bytes(char str[static 1], const char hex[static 1], int hexlen);

/*
 * show every char in a string with their address
 * show printable character and if not printable show number
 */
void show_strmem(char str[static 1], int strlen);

/*
 * free array of pointer
 * if n is -1 then free until null ptr
 */
void free_arrayptr_(string *lines, usize n);
#define free_arrayptr(arr) free_arrayptr_(arr, ARRAY_LEN(arr))

/*
 * free array of pointer
 * if n is -1 then free until null ptr
 * free the array as well
 */
void free_ptrarrayptr(string *array, usize length);
#endif
