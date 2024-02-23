#ifndef FILEDIR_H
#define FILEDIR_H

#include "file.h"

// структура, содержащая список файлов в директории
typedef struct 
{
    // полный путь до директории
    const char* m_path;

    // количество файлов
    int m_length;

    // массив файлов
    File** m_files;

} FileDir;

// печать FileDir
int printFileDir(FileDir* fd);

// создание директории с файлами
FileDir* createFileDir();

// удаление директории с файлами
int deleteFileDir(FileDir* fd);

// установить директорию для каталога
int setPath(FileDir* fd, const char* path);

// получение количества файлов в директории
int countFilesInDirectory(FileDir* fd);

// выделение памяти под массив файлов
int allocateFileArray(FileDir* fd);

// чтение файлов
int readFilesInDir(FileDir* fd);


#endif // !FILEDIR_H