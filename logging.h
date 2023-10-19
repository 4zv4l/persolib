#ifndef LOGGING
#define LOGGING

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include "types.h"
#include "utils.h"

/*
 * log to stderr showing the level and arguments
 * works like printf()
 *
 * ex: log_info("Hello %s !", user);
 */
#define log_info(...)  log_format("INFO",  __VA_ARGS__)
#define log_warn(...)  log_format("WARN",  __VA_ARGS__)
#define log_error(...) log_format("ERROR", __VA_ARGS__)

/*
 * works like printf
 * print to stderr
 * and exit the program with code 255
 */
#define die(...) (fprintf(stderr, __VA_ARGS__), fputc('\n',stderr), exit(255))

/*
 * init the logger files
 * default is logging to stderr
 * return false if failed to initialize
 */
bool init_logger(string *files);

/*
 * free the logger if it has been initialized
 */
void deinit_logger(void);

/*
 * log to stderr and works like printf
 * you can use the macros:
 * `log_info`
 * `log_warn`
 * `log_error`
 *
 * to avoid the tag
 */
void log_format(const char tag[static 4], const char msg[static 1], ...);
#endif
