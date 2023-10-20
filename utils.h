#ifndef UTILS
#define UTILS

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include "types.h"

#ifndef LOGGING
#include "logging.h"
#endif

// foreach for array (known size)
#define foreach(var, arr)                                                   \
    for(                                                                    \
            usize index = 0, done = 1, length = ARRAY_LEN(arr)); \
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

// get stack array length
#define ARRAY_LEN(arr) sizeof((arr))/sizeof((*arr))

/*
 * works like printf
 * print to stderr
 * and exit the program with code 255
 */
#define die(...) (fprintf(stderr, __VA_ARGS__), fputc('\n',stderr), deinit_logger(), exit(255))

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
 * array must be stack array (known size)
 * otherwise use free_ptrarrayptr
 */
#define free_arrayptr(arr) free_arrayptr_(arr, ARRAY_LEN(arr))
void free_arrayptr_(string lines[], usize n);

/*
 * free array of pointer
 * if n is -1 then free until null ptr
 * free the array as well
 */
void free_ptrarrayptr(string array[], usize length);

/*
 * read a line from file f
 * store it in line
 * if line is nullptr then allocate memory
 * otherwise use it or realloc it if not big enough (so no stack array)
 * must be freed afterward (after you finished reading the lines you want)
 *
 * return 0 on error/eof
 */
string fget_line(string *line, FILE f[static 1]);
#endif
