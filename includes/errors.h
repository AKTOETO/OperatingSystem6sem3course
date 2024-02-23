#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>

// возможные ошибки при работе программы
#define OK                          (0)
// ФАЙЛЫ
#define FILE_DOESNT_EXIST           (1 << 1)
#define FILE_HAS_NO_DESCRIPTOR      (1 << 2)
#define FILE_HAS_NO_PATH            (1 << 3)
#define FILE_HAS_NO_SIZE            (1 << 4)
#define FILE_HAS_NO_BUFFER          (1 << 5)
#define FILE_INCORRECT_INPUT_SIZE   (1 << 6)
#define FILE_INCORRECT_OUTPUT_SIZE  (1 << 7)
// ДИРЕКТОРИИ
#define FDIR_DOESNT_EXIST           (1 << 8)
#define FDIR_COULDNT_CREATE_DIR     (1 << 9)
#define FDIR_PATH_DOESNT_EXIST      (1 << 10)
#define FDIR_COULDNT_CHANGE_DIR     (1 << 11)
#define FDIR_COULDNT_CLOSE_DIR      (1 << 12)
#define FDIR_FILES_DOESNT_EXIST     (1 << 13)
#define FDIR_COULDNT_CREATE_FILES   (1 << 14)
#define FDIR_FILE_COUNT_IS_0        (1 << 15)

// расшифровка сообщений об ошибках
#define STR(str) #str
#define CR_ERR_MSG(str, src, er_code) \
if(src & er_code)str = #er_code;

// функция печати ошибок
void errorPrint(int er);

#endif // !ERRORS_H