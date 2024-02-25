#ifndef FILEDIR_H
#define FILEDIR_H

#include "file.h"

// список файлов
extern File** files;

// количество файлов
extern int f_size;

// текущий файл
extern int f_ind;

// создание списка файлов
File** createFileArr(int size);

// удаление списка файлов
int deleteFileArr(File** fa);

// расчет количества файлов
int countFiles(const char *fpath, const struct stat *sb,
               int tflag, struct FTW *ftwbuf);

// добавление файлов в список файлов
int addFile(const char *fpath, const struct stat *sb,
            int tflag, struct FTW *ftwbuf);

#define MAX_FILES_IN_ARCHIVE 200

// создание списка файлов
File** initializeFileArr(char* path);

// печать файлового массива
int printFileArr(File** fa);


#endif // !FILEDIR_H