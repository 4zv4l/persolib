#ifndef REGEX
#define REGEX

#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "types.h"

string *match(char re[static 1], const char str[static 1], u32 *nmatch, u32 opt);

#endif
