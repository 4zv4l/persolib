#include "regex.h"

string*
match(char re[static 1], const char str[static 1], u32 *nmatch, u32 opt)
{
    regex_t regex = {0};
    regmatch_t *matches = malloc(sizeof(regmatch_t) * (*nmatch));
    char **groups = calloc(*nmatch, sizeof(char*));

    if (!matches)
        goto cleanup;
    if (!groups)
        goto cleanup;
    if (regcomp(&regex, re, opt))
        goto cleanup;
    if (regexec(&regex, str, *nmatch, matches, 0))
        goto all_cleanup;

    for (int match = 0; match < *nmatch; match++)
    {
        if (matches[match].rm_so == -1)
        {
            char **tmp = realloc(groups, sizeof(char*) * match);
            if (!tmp)
                goto all_cleanup;

            *nmatch = match;
            groups = tmp;
            break;
        }

        int length = matches[match].rm_eo - matches[match].rm_so;
        groups[match] = malloc(length+1);
        if (!groups[match])
            goto all_cleanup;

        memcpy(groups[match], &str[matches[match].rm_so], length);
        groups[match][length] = 0;
    }

    free(matches);
    regfree(&regex);
    return groups;

all_cleanup:
    regfree(&regex);
cleanup:
    for (int i = 0; i < *nmatch && groups && groups[i]; i++) free(groups[i]);
    free(groups);
    free(matches);
    return 0;
}
