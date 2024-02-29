#ifndef ERRORS_H
#define ERRORS_H

#define _XOPEN_SOURCE 500 // для подключения новых функций стандарта POSIX
                          // nftw
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <ftw.h>        // nftw

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
    printLog(stdout, __FILE__, __LINE__, STR(INFO));\
    fprintf(stdout, format, __VA_ARGS__);

#define ERROR(format, ...)\
    printLog(stderr, __FILE__, __LINE__, STR(ERROR));\
    fprintf(stderr, format, __VA_ARGS__);\
    return EXIT_FAILURE;

#define INFOS(str) INFO("%s\n", str);
#define INFOD(num) INFO("%d\n", num);
#define INFOF(num) INFO("%f\n", num);

#define ERRORS(str) ERROR("%s\n", str);
#define ERRORD(num) ERROR("%d\n", num);
#define ERRORF(num) ERROR("%F\n", num);

#endif // !ERRORS_H