#include "utils.h"

void*
dupcopy(void *data, usize size)
{
    void *copy = malloc(size); 
    if (!copy)
        return 0;

    return memcpy(copy, data, size);
}

usize
ptr_len(void *data, usize size)
{
    usize zero = 0, count = 0;

    for (;;)
    {
        if (memcmp(data, &zero, size) == 0)
            break;
        data+=size;
        count++;
    }

    return count;
}

bool
ends_with(char s[static 1], char c)
{
    usize len = strlen(s);
    if (s[len-1] == c)
        return true;
    return false;
}

string
chomp(char str[static 1])
{
    char *endln = &str[strlen(str)-1];
    if (*endln == '\n')
        *endln = 0;

    return str;
}

string*
split(string str, char sep)
{
    usize ntime      = str_count(str, sep) + 1;
    string *splitted = calloc(ntime+1, sizeof(string));
    string s         = strdup(str), bak = s;

    if (!ntime)
    {
        log_warn("not separator found in string");
        goto cleanup;
    }
    if (!splitted)
    {
        log_warn("couldnt allocate memory for splitted");
        goto cleanup;
    }
    if (!s)
    {
        log_warn("couldnt allocate memory for backup str");
        goto cleanup;
    }

    for (usize i = 0; i < ntime; i++)
    {
        string tok = strtok(s, (char[]){sep, 0});
        if (!tok)
        {
            log_warn("token empty");
            return splitted;
        }
        splitted[i] = strdup(tok);
        if (!splitted[i])
        {
            log_warn("couldnt allocate memory for splitted[%d]", i);
            goto cleanup;
        }

        s = 0;
    }

    free(bak);
    return splitted;

cleanup:
    for (usize i = 0; splitted && splitted[i]; i++)
        free(splitted[i]);
    free(splitted);
    free(s);
    return 0;
}

usize
index_of(char c, char str[static 1])
{
    string tmp = strchr(str, '\n');
    if (tmp)
        return (i32)(tmp - str);

    return 0;
}

usize
str_count(string str, char c)
{
    usize ntime = 0;
    for (usize i = 0; str[i] != 0; i++)
    {
        if (str[i] == c)
            ntime++;
    }
    return ntime;
}

void
bytes_to_hex(char hex[static 1], const char str[static 1], int strlen)
{
    for (int i = 0; i < strlen; i++)
    {
        hex += sprintf(hex, "%02x", str[i]);
    }
}

void
hex_to_bytes(char str[static 1], const char hex[static 1], int hexlen)
{
    for (int i = 0; i < hexlen/2; i++)
    {
        str += sscanf(hex, "%hhx", str);
        hex += 2;
    }
}

void
show_strmem(char str[static 1], int strlen)
{
    for (int i = 0; i <= strlen; i++)
    {
        if (str[i] >= 32 && str[i] <= 126)
            printf("%p -> '%c'\n", &str[i], str[i]);
        else
            printf("%p -> %d\n", &str[i], str[i]);
    }
}

void
free_arrayptr_(string array[], usize length)
{
    if (length == -1)
    {
        for (usize i = 0; array[i] != 0; i++)
            free(array[i]);
    } else
    {
        for (usize i = 0; i < length; i++)
            free(array[i]);
    }
}

void
free_ptrarrayptr(string array[], usize length)
{
    if (length == -1)
    {
        for (usize i = 0; array[i] != 0; i++)
            free(array[i]);
    } else
    {
        for (usize i = 0; i < length; i++)
            free(array[i]);
    }
    free(array);
}

string
fget_line(string *line, FILE f[static 1])
{
    u64 length = *line ? strlen(*line) : 0;
    if (getline(line, &length, f) == -1)
        return 0;
    return *line;
}
