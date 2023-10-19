#ifndef REGEX
#define REGEX

#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "types.h"
#include "logging.h"

/*
 * helper for match_
 */
// default using REG_EXTENDED
#define match_defltopt(re, str, groups) match_(re, str, groups, ARRAY_LEN(groups), REG_EXTENDED)
#define match_opt(re, str, groups, opt) match_(re, str, groups, ARRAY_LEN(groups), opt)
#define match_select(_1, _2, _3, _4, FUNC, ...) FUNC


/*
 * put matched data in groups (must be stack array)
 * groups must be the size of match you want to get back
 * return true if match
 * return false if doesnt match
 *
 * strings stored in groups must be freed
 */
#define match(...) match_select(__VA_ARGS__, match_opt, match_defltopt)(__VA_ARGS__)
bool match_(char re[static 1], const char str[static 1], string *groups, usize ngroup, u32 opt);
#endif
