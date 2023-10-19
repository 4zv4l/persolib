#include "regex.h"

bool
match_(char re[static 1], const char str[static 1], string *groups, usize ngroup, u32 opt)
{
    regex_t regex = {0};
    usize match = {0};
    regmatch_t *matches = malloc(sizeof(regmatch_t) * ngroup);

    if (!matches)
    {
        log_warn("malloc(): couldnt alloc memory for matches");
        goto cleanup;
    }
    if (regcomp(&regex, re, opt))
    {

        log_warn("regcomp(): '%s' couldnt compile", re);
        goto cleanup;
    }
    if (regexec(&regex, str, ngroup, matches, 0))
    {
        log_warn("regexec(): '%s' doesnt match", str);
        goto all_cleanup;
    }
    log_info("regexec(): '%s' match", str);

    for (match = 0; match < ngroup; match++)
    {
        if (matches[match].rm_so == -1)
            break;

        int length = matches[match].rm_eo - matches[match].rm_so;
        groups[match] = malloc(length+1);
        if (!groups[match])
        {
            log_warn("malloc(): couldnt alloc memory for groups[%d]", match);
            goto all_cleanup;
        }

        memcpy(groups[match], &str[matches[match].rm_so], length);
        groups[match][length] = 0;
    }

    free(matches);
    regfree(&regex);
    return true;

all_cleanup:
    regfree(&regex);
cleanup:
    for (int i = 0; i < match; i++)
        free(groups[i]);
    free(matches);
    return false;
}
