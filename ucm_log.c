#include "ucm_log.h"

void fatal(const int err, const char *fmt, ...)
{
    errno = err == 0 ? -1 : err;
    va_list val_list;
    char buf[TMP_MAX];

    va_start(val_list, fmt);
    vsprintf(buf, fmt, val_list);
    printf("\033[31mFATAL: %s\033[0m\n", buf);
    va_end(val_list);

    exit(errno);
}

void warn(const char *fmt, ...)
{
    va_list val_list;
    char buf[TMP_MAX];

    va_start(val_list, fmt);
    vsprintf(buf, fmt, val_list);
    printf("\033[33mWARN: %s\033[0m\n", buf);
    va_end(val_list);
}