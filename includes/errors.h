#ifndef ERRORS_H
#define ERRORS_H

#include "includes.h"

// цвета сообщений
#define DEFAULT_WHITE    "\033[0m"
#define ERROR_BOLD_RED   "\033[1;31m"
#define INFO_BOLD_YELLOW "\033[1;33m"

// нет ошибок
#define OK EXIT_SUCCESS

// разрешено ли использование INFO
extern bool g_use_info;

// поиск имени файла
size_t findFileNamePos(const char* str);

// печать справочной информации
void printLog(
    FILE *out_stream, int pid, 
    const char* file, const char* func,
    int line, const char* type);

#define STR(str) #str
#define INFO(format, ...)\
{\
    if(g_use_info) \
    {\
        printLog(stdout, getpid(), __FILE__, __FUNCTION__, __LINE__, STR(INFO));\
        fprintf(stdout, format, __VA_ARGS__);\
    }\
}

#define ERROR(format, ...) \
{\
    printLog(stderr, getpid(), __FILE__, __FUNCTION__, __LINE__, STR(ERROR));\
    fprintf(stderr, format, __VA_ARGS__);\
}

#define INFOS(str) INFO("%s", str);
#define INFOD(num) INFO("%d", num);
#define INFOF(num) INFO("%f", num);

#define ERRORS(str) ERROR("%s", str);
#define ERRORD(num) ERROR("%d", num);
#define ERRORF(num) ERROR("%F", num);

#endif // !ERRORS_H