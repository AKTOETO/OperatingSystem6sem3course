#ifndef ERRORS_H
#define ERRORS_H

#define _XOPEN_SOURCE 500 // для подключения новых функций стандарта POSIX
                          // nftw
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <ftw.h>        // nftw

// цвета сообщений
#define DEFAULT_WHITE    "\033[0m"
#define ERROR_BOLD_RED   "\033[1;31m"
#define INFO_BOLD_YELLOW "\033[1;33m"

// нет ошибок
#define OK EXIT_SUCCESS

// поиск имени файла
size_t findFileNamePos(const char* str);

// печать справочной информации
void printLog(
    FILE *out_stream, const char* file,
    int line, const char* type);

#define STR(str) #str
#define INFO(format, ...)\
{\
    printLog(stdout, __FILE__, __LINE__, STR(INFO));\
    fprintf(stdout, format, __VA_ARGS__);\
}

#define ERROR(format, ...) \
{\
    printLog(stderr, __FILE__, __LINE__, STR(ERROR));\
    fprintf(stderr, format, __VA_ARGS__);\
    return EXIT_FAILURE;\
}

#define INFOS(str) INFO("%s", str);
#define INFOD(num) INFO("%d", num);
#define INFOF(num) INFO("%f", num);

#define ERRORS(str) ERROR("%s", str);
#define ERRORD(num) ERROR("%d", num);
#define ERRORF(num) ERROR("%F", num);

#endif // !ERRORS_H