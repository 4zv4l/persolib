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

string
chomp(char str[static 1])
{
    char *endln = &str[strlen(str)-1];
    if (*endln == '\n')
        *endln = 0;
    return str;
}

usize
index_of(char c, char str[static 1])
{
    string tmp = strchr(str, '\n');
    if (tmp) return (i32)(tmp - str);
    return 0;
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
