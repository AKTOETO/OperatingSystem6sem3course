#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

#include <memory>

// открытие файла, из которого читаем
int openInputFile(const char* path)
{
    // открытие файла
    int in = open(path, O_RDONLY);

    // если файл не был открыт
    if(in == -1)
    {
        printf("Файл %s не был открыт\n", path);
        exit(-1);
    }

    printf("Файл %s открыт\n", path);

    return in;
}

// открытие файла, в который пишем
int openOutputFile(const char* path)
{
    // открытие файла
    int out = open(path, O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);

    // если файл не был открыт
    if(out == -1)
    {
        printf("Файл %s не был открыт\n", path);
        exit(-1);
    }
        
    printf("Файл %s открыт\n", path);

    return out;
}

// определение ращмера файла
int getFileSize(int file)
{
    // определение размера файла
    int file_size = lseek(file, 0, SEEK_END);
    printf("Размер файла %d\n", file_size);
    
    // ставим курсор в начало файла
    lseek(file, 0, SEEK_SET);

    return file_size;
}

// создание блока для считывания файла
char* createBlock(int size)
{
    // блок для считывания файла
    char* block = new char[size + 1];
    block[size] = '\0';
    return block;
}

// копирование файла в выделенный блок
int copyFileToBlock(int file, char* block, int size)
{
    // количество считанных байт
    int read_size = 0;

    // поблочное копирование файла
    read_size = read(file, block, size);
    
    printf("Считанный блок данных в %d байт:\n %s \n", read_size, block);

    return read_size;
}

// структура файла
struct file
{
    // идентификатор файла
    int m_id;

    // путь к файлу
    const char* m_path;

    // информация в нем
    char* m_buffer;

    // размер буфера
    int m_size;

    file(const char* _path):
    m_id(-1), m_path(_path), m_buffer(nullptr), m_size(0)
    {}

    ~file()
    {
        //delete[] m_path;
        delete[] m_buffer;
    }
};

using pFile = std::unique_ptr<file>;

// чтение файла
pFile readInputFile(const char* path)
{
    // создаем файл
    pFile f = std::make_unique<file>(path);

    // открытие файла
    f->m_id = openInputFile(path);

    // определение размера файла
    f->m_size = getFileSize(f->m_id);

    // блок для считывания файла
    f->m_buffer = createBlock(f->m_size);

    // копируем информацию из блока в файл
    copyFileToBlock(f->m_id, f->m_buffer, f->m_size);

    return f;
}

// функция тестирования работоспособности readInputFile
void testReadInputFile(int argc, char** argv)
{
    // путь к файлу, который будем читать
    const char* file_path = argv[1];
    // входной и выходной файл
    int out;

    // создание входного файла
    pFile in = readInputFile(file_path);

    // открытие файла
    out = openOutputFile("file.out");

    // печатаем всю информацию из блока в выходной файл
    write(out, in->m_buffer, in->m_size);

    close(in->m_id);
    close(out);
}

// печать каталога
void printDir(const char* path, int depth)
{
    //поток каталога
    DIR* dir;

    // элемент каталога
    dirent* entry;

    // для сборки информации
    struct stat statbuf;

    // пытаемся открыть каталог
    if((dir = opendir(path)) == NULL)
    {
        fprintf(stderr, "Директория %s не была открыта", path);
        return;
    }

    // переходим в открытую директорию
    chdir(path);

    // проходимся по всем элементам этой директории
    while((entry = readdir(dir)) != NULL)
    {
        // получение справочной информации о текущем элементе каталога
        lstat(entry->d_name, &statbuf);

        // если текущий элемент - каталог
        if(S_ISDIR(statbuf.st_mode))
        {
            // игнорируем каталоги "." и ".."
            if(strcmp(".",entry->d_name) == 0 ||
               strcmp("..", entry->d_name) == 0)
                continue;
            
            printf("%*s%s/\n", depth, " ", entry->d_name);

            // вызываем рекурсивную печать от текущего каталога
            printDir(entry->d_name, depth+4);
        }
        // если же это файл
        else
        {
            printf("%*s%s\n", depth, " ", entry->d_name);
        }
    }

    // переходим в директорию выше
    chdir("..");

    // закрываем поток каталога
    closedir(dir);
}

int main(int argc, char** argv)
{
    // получение пути к файлу, который надо считать, 
    // через аргументы argv
    if(argc < 2)
    {
        printf("Недостаточно аргументов (нужен еще путь к файлу)\n");
        return 1;
    }

    // testReadInputFile(argc, argv);
    
    printDir(argv[1], 0);
    

    return 0;
}
