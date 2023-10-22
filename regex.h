#ifndef REGEX
#define REGEX

#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "types.h"

#ifndef LOGGING
#include "logging.h"
#endif

/*
 * put matched data in groups
 * will capture at most `length`
 * return true if match
 * return false if doesnt match
 *
 * strings stored in groups must be freed
 */
#define match(re, str, groups, length) match_(re, str, groups, length, REG_EXTENDED)
bool match_(char re[static 1], const char str[static 1], string *groups, usize ngroup, u32 opt);
#endif
