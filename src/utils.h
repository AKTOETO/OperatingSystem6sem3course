#ifndef UTILS_H
#define UTILS_H

#include "includes.h"

// проверка строки: заканчивается ли строка str на end
static inline bool strEndIn(const char *str, const char *end)
{
    size_t str_len = strlen(str);
    size_t end_len = strlen(end);

    char *pos = strstr(str, end);
    return (pos != NULL) && (pos + end_len == str + str_len);
}

// проверка строки: заканчивается ли строка str на один элемент из end
static inline bool strEndArrIn(const char *str, const char *end[], size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        if(strEndIn(str, end[i]) == true)
            return true;
    }
    return false;
}

#endif // !UTILS_H