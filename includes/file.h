#ifndef FILE_H
#define FILE_H

#include "errors.h"

// структура файла
typedef struct
{
    int m_id; // дескриптор файла
    
    char* m_path; // путь к файлу

    size_t m_path_size; // размер пути к файлу
    
    char* m_buffer; // информация в нем
    
    size_t m_size; // размер буфера

} File;

// проверки для выполнения
#define FILE_EXISTANCE              if(f == NULL) return FILE_DOESNT_EXIST
#define FILE_PATH_EXISTANCE         if(f->m_path == NULL)return FILE_HAS_NO_PATH
#define FILE_DESCRIPTOR_EXISTANCE   if(f->m_id == -1)return FILE_HAS_NO_DESCRIPTOR
#define FILE_SIZE_EXISTANCE         if(f->m_size == -1)return FILE_HAS_NO_SIZE
#define FILE_BUFER_EXISTANCE        if(f->m_buffer == NULL)return FILE_HAS_NO_BUFFER


// печать информации о файле
int printFile(File* f);

// функции обработки структуры File
// создание файла
File* createFile();

// удаление файла
void deleteFile(File* f);

// установить файловый путь 
int setFilepath(File* f, const char* path);

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