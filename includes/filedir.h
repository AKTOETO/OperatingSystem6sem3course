#ifndef FILEDIR_H
#define FILEDIR_H

#include "file.h"

// структура, содержащая список файлов в директории
typedef struct 
{
    // количество файлов
    int m_length;

    // массив файлов
    File* m_files;

} FileDir;

// создание директории с файлами
FileDir* createFileDir();

// удаление директории с файлами
void deleteFileDir(FileDir* f);


#endif // !FILEDIR_H