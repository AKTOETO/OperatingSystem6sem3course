#include "file.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

// функции обработки структуры file
// создание файла
file* createFile()
{
    file* f = (file*)calloc(1, sizeof(file));

    // значения по умолчанию
    f->m_id = -1;
    f->m_buffer = NULL;
    f->m_size = -1;
    f->m_path = "";

    return f;
}

// печать информации о файле
int printFile(file* f)
{
    if(f == NULL)
        return FILE_DOESNT_EXIST;

    // если есть путь
    if(strcmp(f->m_path, "") == 0)
    {
        return FILE_HAS_NO_PATH;
    }
    // если есть дескриптор файла
    if(f->m_id == -1)
    {
        return FILE_HAS_NO_DESCRIPTOR;
    }

    // если есть размер файла
    if(f->m_size == -1)
    {
        return FILE_HAS_NO_SIZE;
    }

    // если есть буфер файла
    if(f->m_buffer == NULL)
    {
        return FILE_HAS_NO_BUFFER;
    }

    printf("\nFILE INFO\n");
    printf("path  : <%s>\n", f->m_path);
    printf("id    : <%d>\n", f->m_id);
    printf("size  : <%d>\n", f->m_size);
    printf("buffer: <%s>\n", f->m_buffer);

    return OK;
}

// удаление файла
void deleteFile(file* f)
{
    // удаление буфера
    if(f->m_buffer != NULL)
        free(f->m_buffer);

    // удаление структуры файла
    free(f);
}

// установить файловый путь 
int setFilepath(file* f, const char* path)
{
    // если файл не создан
    if(f == NULL)
    {
        return FILE_DOESNT_EXIST;
    }

    // сохранение файлового пути
    f->m_path = path;

    return OK;
}

// создание файлового дескриптора
// для файла чтения
int openInputFile(file* f)
{
    // если файл не создан
    if(f == NULL)
    {
        return FILE_DOESNT_EXIST;
    }

    // если нет файлового пути
    if(strcmp(f->m_path, "") == 0)
    {
        return FILE_HAS_NO_PATH;
    }

    // создание файлового дескриптора
    f->m_id = open(f->m_path, O_RDONLY);

    // если же дескриптор не был создан
    if(f->m_id == -1)
    {
        return FILE_HAS_NO_DESCRIPTOR;
    }

    return OK;
}

int openOutputFile(file *f)
{
    // если файл не создан
    if(f == NULL)
    {
        return FILE_DOESNT_EXIST;
    }

    // если нет файлового пути
    if(strcmp(f->m_path, "") == 0)
    {
        return FILE_HAS_NO_PATH;
    }

    // создание файлового дескриптора
    f->m_id = open(f->m_path, O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);

    // если же дескриптор не был создан
    if(f->m_id == -1)
    {
        return FILE_HAS_NO_DESCRIPTOR;
    }

    return OK;
}

// закрытие файла
int closeFile(file* f)
{
    // если файл не создан
    if(f == NULL)
    {
        return FILE_DOESNT_EXIST;
    }

    // разрыв связи файлового дескриптора с файлом
    close(f->m_id);

    return OK;
}

// чтение размера файла
int readFileSize(file* f)
{
    // если файл не создан
    if(f == NULL)
    {
        return FILE_DOESNT_EXIST;
    }

    // если не создан файловый дескриптор
    if(f->m_id == -1)
    {
        return FILE_HAS_NO_DESCRIPTOR;
    }

    // создание структуры для чтения информации о файле
    struct stat st;

    // чтение информации о файле
    fstat(f->m_id, &st);

    // установление размера файла
    f->m_size = st.st_size;

    return OK;
}

// чтение буфера файла
int readFileBuffer(file* f)
{
    // если файл не создан
    if(f == NULL)
    {
        return FILE_DOESNT_EXIST;
    }

    // если не задан размер файла
    if(f->m_size == -1)
    {
        return FILE_HAS_NO_SIZE;
    }

    // создание файлового буфера 
    f->m_buffer = (char*)malloc((f->m_size + 1) * sizeof(char));

    // если выделить память не удалось
    if(f->m_buffer == NULL)
    {
        return FILE_HAS_NO_BUFFER;
    }

    f->m_buffer[f->m_size] = '\0';

    // чтение буфера файла
    int read_size = read(f->m_id, f->m_buffer, f->m_size);

    // если считанное количество байт не равно размеру буфера, 
    // выводим ошибку
    if(read_size != f->m_size)
    {
        return FILE_INCORRECT_INPUT_SIZE;
    }

    return OK;
}

int writeFile(file *f)
{
    // если файл не создан
    if(f == NULL)
    {
        return FILE_DOESNT_EXIST;
    }
    // если есть путь
    if(strcmp(f->m_path, "") == 0)
    {
        return FILE_HAS_NO_PATH;
    }
    // если есть дескриптор файла
    if(f->m_id == -1)
    {
        return FILE_HAS_NO_DESCRIPTOR;
    }

    // если есть размер файла
    if(f->m_size == -1)
    {
        return FILE_HAS_NO_SIZE;
    }

    // если есть буфер файла
    if(f->m_buffer == NULL)
    {
        return FILE_HAS_NO_BUFFER;
    }

    // записываем сколько байт записалось
    int write_bytes = write(f->m_id, f->m_buffer, f->m_size);

    // если количество байт = -1 или 
    // размер файла не равен количеству распечатанных байт,
    // значит произошла ошибка
    if(write_bytes == -1 || write_bytes != f->m_size)
    {
        return FILE_INCORRECT_OUTPUT_SIZE;
    }


    return OK;
}
