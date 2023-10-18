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
