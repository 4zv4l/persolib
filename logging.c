#include "logging.h"

// array of FILE to log to, default is stderr
string *LOG_FILES = (string[]){ 0 };

void
init_logger(string *files)
{
    if (files)
    {
        usize length = ptr_len(files, sizeof(files));
        string *tmp = malloc(sizeof(char*) * (length + 1));
        for (usize i = 0; i < length; i++)
            tmp[i] = strdup(files[i]);
        tmp[length] = 0;
        LOG_FILES = tmp;
    }
}

void
log_format(const char tag[static 4], const char msg[static 1], ...)
{
    va_list args;
    va_start(args, msg);
    char buff[2048] = {0};
    vsprintf(buff, msg, args);
    va_end(args);

    time_t now;
    time(&now);
    struct tm *timeinfo = localtime(&now);

    char date[256] = {0};
    strftime(date, sizeof(date), "%FT%T", timeinfo);

    pforeach(filename, LOG_FILES) {
        FILE *out;
    
        if (strcmp(filename, "stderr") == 0) out = stderr;
        else out = fopen(filename, "a+");

        if (!out)
        {
            fprintf(stderr, "log error %s: %s", filename, strerror(errno));
            continue;
        }

        fprintf(out, "%s [%s] ", date, tag);
        fprintf(out, "%s", buff);
        fprintf(out, "\n");
        
        fclose(out);
    }
}
