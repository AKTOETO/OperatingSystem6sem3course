#ifndef FILE_H
#define FILE_H

#include "errors.h"

// структура файла
typedef struct
{
    int m_id; // дескриптор файла
    
    const char* m_path; // путь к файлу
    
    char* m_buffer; // информация в нем
    
    int m_size; // размер буфера

} file;

// печать информации о файле
int printFile(file* f);

// функции обработки структуры file
// создание файла
file* createFile();

// удаление файла
void deleteFile(file* f);

// установить файловый путь 
int setFilepath(file* f, const char* path);

// создание файлового дескриптора
// для файла чтения
int openInputFile(file* f);

// создание файлового дескриптора для печати файла
int openOutputFile(file* f);

// закрытие файла
int closeFile(file* f);

// чтение размера файла
int readFileSize(file* f);

// чтение буфера файла
int readFileBuffer(file* f);

// вывести содержимое буфера в файл
int writeFile(file* f);

#endif // !FILE_H