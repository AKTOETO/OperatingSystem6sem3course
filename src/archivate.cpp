#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

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

int main(int argc, char** argv)
{
    // получение пути к файлу, который надо считтать, 
    // через аргументы argv
    if(argc < 2)
    {
        printf("Недостаточно аргументов (нужен еще путь к файлу)\n");
        return 1;
    }

    // путь к файлу, который будем читать
    char* file_path = argv[1];
    // входной и выходной файл
    int in, out;

    // открытие файла
    in = openInputFile(file_path);
    out = openOutputFile("file.out");

    // определение размера файла
    int file_size = getFileSize(in);

    // блок для считывания файла
    char* block = createBlock(file_size);

    // копируем информацию из блока в файл
    copyFileToBlock(in, block, file_size);

    // печатаем всю информацию из блока в выходной файл
    write(out, block, file_size);

    // чистка памяти
    delete[] block;

    return 0;
}
