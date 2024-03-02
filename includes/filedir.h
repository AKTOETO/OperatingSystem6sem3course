#ifndef FILEDIR_H
#define FILEDIR_H

#include "file.h"

// список файлов
extern File** files;

// количество файлов
extern int f_size;

// текущий файл
extern int f_ind;

// базовая папка
extern char* base_folder;

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
File** fillFileArrFromDir(char* path);

// печать файлового массива
int printFileArr(File** fa);

// создание директории, в которой должен быть файл
char* createFileDir(const char* dir);

// рекурсивное создание папок
void recursiveMkdir(char *path);

// разархивирование одного файла
void dearchivateFile(File* f);

// заполнение файловой системы файлами
void fillDirFromFileArr(File** fa);

// ПРОВЕРКИ

// получение значени типа объекта (файл или директория)
int getStMode(const char *path);


#endif // !FILEDIR_H