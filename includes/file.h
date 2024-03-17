#ifndef FILE_H
#define FILE_H

#include "errors.h"

// структура файла
typedef struct
{
    int m_id;           // дескриптор файла    
    char* m_path;       // путь к файлу
    size_t m_path_size; // размер пути к файлу    
    char* m_buffer;     // информация в нем    
    size_t m_size;      // размер буфера
} File;

// проверки для выполнения
#define FILE_EXISTANCE              {if(f == NULL) ERRORS("файл не существует")}
#define FILE_PATH_EXISTANCE         {if(f->m_path == NULL) ERRORS("у файла нет пути")}
#define FILE_DESCRIPTOR_EXISTANCE   {if(f->m_id == -1) ERRORS("у файла нет дескриптора")}
#define FILE_SIZE_EXISTANCE         {if(f->m_size == -1) ERRORS("у файла нет размера")}
#define FILE_BUFER_EXISTANCE        {if(f->m_buffer == NULL) ERRORS("у файла нет буфера")}


// печать информации о файле
int printFile(File* f);

// функции обработки структуры File
// создание файла
File* createFile();

// удаление файла
int deleteFile(File* f);

// установить файловый путь 
int setFilepath(File* f, const char* path);

// установить буфер файла
int setFileBufferSize(File* f, const char* buffer, size_t size);

// установить буфер файла
int setFileBuffer(File* f, const char* buffer);

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