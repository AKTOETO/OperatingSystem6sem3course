#include "file.h"


// функции обработки структуры File
// создание файла
File* createFile()
{
    File* f = (File*)calloc(1, sizeof(File));

    // значения по умолчанию
    f->m_id = -1;
    f->m_buffer = NULL;
    f->m_size = -1;
    f->m_path = "";

    return f;
}

// печать информации о файле
int printFile(File* f)
{
    // существование файла
    FILE_EXISTANCE;

    // если есть путь
    FILE_PATH_EXISTANCE;

    // если есть дескриптор файла
    FILE_DESCRIPTOR_EXISTANCE;

    // если есть размер файла
    FILE_SIZE_EXISTANCE;

    // если есть буфер файла
    FILE_BUFER_EXISTANCE;

    printf("\nFILE INFO\n");
    printf("path  : <%s>\n", f->m_path);
    printf("id    : <%d>\n", f->m_id);
    printf("size  : <%d>\n", f->m_size);
    printf("buffer: <%s>\n", f->m_buffer);

    return OK;
}

// удаление файла
void deleteFile(File* f)
{
    // удаление буфера
    if(f->m_buffer != NULL)
        free(f->m_buffer);

    // удаление структуры файла
    free(f);
}

// установить файловый путь 
int setFilepath(File* f, const char* path)
{
    // если файл не создан
    FILE_EXISTANCE;

    // сохранение файлового пути
    f->m_path = path;

    return OK;
}

// создание файлового дескриптора
// для файла чтения
int openInputFile(File* f)
{
    // если файл не создан
    FILE_EXISTANCE;

    // если нет файлового пути
    FILE_PATH_EXISTANCE;

    // создание файлового дескриптора
    f->m_id = open(f->m_path, O_RDONLY);

    // если же дескриптор не был создан
    FILE_DESCRIPTOR_EXISTANCE;

    return OK;
}

int openOutputFile(File *f)
{
    // если файл не создан
    FILE_EXISTANCE;

    // если нет файлового пути
    FILE_PATH_EXISTANCE;

    // создание файлового дескриптора
    f->m_id = open(f->m_path, O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);

    // если же дескриптор не был создан
    FILE_DESCRIPTOR_EXISTANCE;

    return OK;
}

// закрытие файла
int closeFile(File* f)
{
    // если файл не создан
    FILE_EXISTANCE;

    // разрыв связи файлового дескриптора с файлом
    close(f->m_id);

    return OK;
}

// чтение размера файла
int readFileSize(File* f)
{
    // если файл не создан
    FILE_EXISTANCE;

    // если не создан файловый дескриптор
    FILE_DESCRIPTOR_EXISTANCE;

    // создание структуры для чтения информации о файле
    struct stat st;

    // чтение информации о файле
    fstat(f->m_id, &st);

    // установление размера файла
    f->m_size = st.st_size;

    return OK;
}

// чтение буфера файла
int readFileBuffer(File* f)
{
    // если файл не создан
    FILE_EXISTANCE;

    // если не задан размер файла
    FILE_SIZE_EXISTANCE;

    // создание файлового буфера 
    f->m_buffer = (char*)malloc((f->m_size + 1) * sizeof(char));

    // если выделить память не удалось
    FILE_BUFER_EXISTANCE;

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

int writeFile(File *f)
{
    FILE_EXISTANCE;
    FILE_PATH_EXISTANCE; 
    FILE_DESCRIPTOR_EXISTANCE;
    FILE_SIZE_EXISTANCE;       
    FILE_BUFER_EXISTANCE;        

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
