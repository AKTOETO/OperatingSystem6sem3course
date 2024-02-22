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

} File;

// печать информации о файле
int printFile(File* f);

// функции обработки структуры File
// создание файла
File* createFile();

// удаление файла
void deleteFile(File* f);

// установить файловый путь 
int setFilepath(File* f, const char* path);

// создание файлового дескриптора
// для файла чтения
int openInputFile(File* f);

// создание файлового дескриптора для печати файла
int openOutputFile(File* f);

// закрытие файла
int closeFile(File* f);

// чтение размера файла
int readFileSize(File* f);

// чтение буфера файла
int readFileBuffer(File* f);

// вывести содержимое буфера в файл
int writeFile(File* f);

#endif // !FILE_H