#define _XOPEN_SOURCE 500 // для подключения новых функций стандарта POSIX
                          // nftw

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <dirent.h>
#include <string.h>
#include <ftw.h>

#include "file.h"

// // открытие файла, из которого читаем
// int openInputFile(const char* path)
// {
//     // открытие файла
//     int in = open(path, O_RDONLY);

//     // если файл не был открыт
//     if(in == -1)
//     {
//         printf("Файл %s не был открыт\n", path);
//         exit(-1);
//     }

//     printf("Файл %s открыт\n", path);

//     return in;
// }

// // открытие файла, в который пишем
// int openOutputFile(const char* path)
// {
//     // открытие файла
//     int out = open(path, O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);

//     // если файл не был открыт
//     if(out == -1)
//     {
//         printf("Файл %s не был открыт\n", path);
//         exit(-1);
//     }
        
//     printf("Файл %s открыт\n", path);

//     return out;
// }

// // определение ращмера файла
// int getFileSize(int File)
// {
//     // определение размера файла
//     int file_size = lseek(File, 0, SEEK_END);
//     printf("Размер файла %d\n", file_size);
    
//     // ставим курсор в начало файла
//     lseek(File, 0, SEEK_SET);

//     return file_size;
// }

// // создание блока для считывания файла
// char* createBlock(int size)
// {
//     // блок для считывания файла
//     char* block =  (char*)malloc((size + 1) * sizeof(char));
//     block[size] = '\0';
//     return block;
// }

// // копирование файла в выделенный блок
// int copyFileToBlock(int File, char* block, int size)
// {
//     // количество считанных байт
//     int read_size = 0;

//     // поблочное копирование файла
//     read_size = read(File, block, size);
    
//     printf("Считанный блок данных в %d байт:\n %s \n", read_size, block);

//     return read_size;
// }

// // структура файла
// struct File
// {
//     // идентификатор файла
//     int m_id;

//     // путь к файлу
//     const char* m_path;

//     // информация в нем
//     char* m_buffer;

//     // размер буфера
//     int m_size;
// };

// // инициализация файла
// struct File* initFile()
// {
//     // выделение памяти под файл
//     struct File* f = (struct File*)malloc(sizeof(struct File));

//     // заполнение файла стандартными значениями
//     *f = (struct File){-1, NULL, NULL, 0};

//     return f;
// }

// struct File* initFileIdPath(int id, const char* path)
// {
//     // выделение памяти под файл
//     struct File* f = (struct File*)malloc(sizeof(struct File));

//     // заполнение файла стандартными значениями
//     *f = (struct File){id, path, NULL, 0};

//     return f;
// }

// void deleteFile(struct File* f)
// {
//     // if(!f->m_path)
//     //     free(f->m_path);
    
//     if(f->m_buffer)
//         free(f->m_buffer);

//     // закрытие файлового потока
//     close(f->m_id);

//     free(f);
// }

// typedef struct File* pFile;

// // чтение файла
// pFile readInputFile(const char* path)
// {
//     // создаем файл;
//     // сохранение пути и открытие файла
//     pFile f = initFileIdPath(openInputFile(path), path);

//     // определение размера файла
//     f->m_size = getFileSize(f->m_id);

//     // блок для считывания файла
//     f->m_buffer = createBlock(f->m_size);

//     // копируем информацию из блока в файл
//     copyFileToBlock(f->m_id, f->m_buffer, f->m_size);
//     printf("PATH: : : :: %s\n", f->m_path);

//     return f;
// }

// // функция тестирования работоспособности readInputFile
// void testReadInputFile(int argc, char** argv)
// {
//     // путь к файлу, который будем читать
//     const char* file_path = argv[1];
//     // входной и выходной файл
//     int out;

//     // создание входного файла
//     pFile in = readInputFile(file_path);

//     // открытие файла
//     out = openOutputFile("File.out");

//     // печатаем всю информацию из блока в выходной файл
//     write(out, in->m_buffer, in->m_size);

//     deleteFile(in);

//     //close(in->m_id);
//     close(out);
// }

// static pFile files[20];
// static int ptr = 0;

// // печать каталога
// void listDir(const char* path, int depth)
// {
//     //printf("\t\tpath: %s\n", path);

//     // поток каталога
//     DIR* dir;

//     // элемент каталога
//     struct dirent* entry;

//     // для сборки информации
//     struct stat statbuf;

//     // пытаемся открыть каталог
//     if((dir = opendir(path)) == NULL)
//     {
//         fprintf(stderr, " Директория %s не была открыта\n", path);
//         return;
//     }

//     // переходим в открытую директорию
//     chdir(path);

//     // проходимся по всем элементам этой директории
//     while((entry = readdir(dir)) != NULL)
//     {
//         // получение справочной информации о текущем элементе каталога
//         lstat(entry->d_name, &statbuf);
        
//         // полный путь
//         char full_path[1024];
//         snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        
//         // если текущий элемент - каталог
//         if(S_ISDIR(statbuf.st_mode))
//         {
//             // игнорируем каталоги "." и ".."
//             if(strcmp(".",entry->d_name) == 0 ||
//                strcmp("..", entry->d_name) == 0)
//                 continue;
            
//             //printf("%*s%s/\n", depth, " ", entry->d_name);

//             // вызываем рекурсивную печать от текущего каталога
//             listDir(full_path, depth+4);
//         }
//         // если же это файл
//         else
//         {
//             printf("\t fullpath for \"%s\" is \"%s\"\n", entry->d_name, full_path);
//             //printf("%*s%s\n", depth, " ", entry->d_name);

//             // добавляем файл 
//             files[ptr++] = readInputFile(full_path);
//             printf("PATH ==== %s\n", files[ptr-1]->m_path);
//         }
//     }

//     // переходим в директорию выше
//     chdir("..");

//     // закрываем поток каталога
//     closedir(dir);
// }

void testFileFunc(int argc, char** argv)
{
    if(argc < 3)
    {
        printf("Недостаточно аргументов (нужен еще путь к файлу)\n");
        return;
    }

    // создание файла
    File* f = createFile();

    errorPrint(setFilepath(f, argv[1]));
    errorPrint(openInputFile(f));
    errorPrint(readFileSize(f));
    errorPrint(readFileBuffer(f));
    
    errorPrint(printFile(f));

    // создаю дескриптор выходного файла
    errorPrint(setFilepath(f, argv[2]));
    errorPrint(openOutputFile(f));
    errorPrint(writeFile(f));
    
    errorPrint(printFile(f));

    errorPrint(closeFile(f));
    deleteFile(f);
}

// void testFileDirRead(int argc, char** argv)
// {
//     // получение пути к файлу, который надо считать, 
//     // через аргументы argv
//     if(argc < 2)
//     {
//         printf("Недостаточно аргументов (нужен еще путь к файлу)\n");
//         return;
//     }

//     // тестирование считывания файлов из директории
//     FileDir* fd = createFileDir();
//     errorPrint(setPath(fd, argv[1]));
//     errorPrint(countFilesInDirectory(fd));
//     errorPrint(allocateFileArray(fd));
//     errorPrint(readFilesInDir(fd));

//     errorPrint(printFileDir(fd));

//     errorPrint(deleteFileDir(fd));
// }

int display_info(const char *fpath, const struct stat *sb,
             int tflag, struct FTW *ftwbuf)
{
    printf("%-3s %2d %7jd   %-90s %d %s\n",
        (tflag == FTW_D) ?   "d"   : (tflag == FTW_DNR) ? "dnr" :
        (tflag == FTW_DP) ?  "dp"  : (tflag == FTW_F) ?   "f" :
        (tflag == FTW_NS) ?  "ns"  : (tflag == FTW_SL) ?  "sl" :
        (tflag == FTW_SLN) ? "sln" : "???",
        ftwbuf->level, (intmax_t) sb->st_size,
        fpath, ftwbuf->base, fpath + ftwbuf->base);

    return 0;           /* To tell nftw() to continue */
}

// список файлов
File** files;
// количество файлов
int f_size = 0;
// текущий файл
int f_ind = 0;

// создание списка файлов
File** createFileArr(int size)
{
    return (File**)calloc(size, sizeof(File*));
}

// удаление списка файлов
int deleteFileArr(File** fa)
{
    // если списка файлов не существует 
    if(fa == NULL)
    {
        return FARR_DOESNT_EXIST;
    }

    // проходимся по всему списку файлов
    for(int i = 0; i < f_size; i++)
    {
        // если существует файл, удаляем его
        if(fa[i] != NULL)
        {
            closeFile(fa[i]);
            deleteFile(fa[i]);
        }
    }

    // удаление массива файлов
    free(fa);

    return OK;
}

// расчет количества файлов
int countFiles(const char *fpath, const struct stat *sb,
               int tflag, struct FTW *ftwbuf)
{
    // если текущий элемент - файл
    // увеличиваем количество файлов
    if(tflag == FTW_F)
    {
        // ВРЕМЕННАЯ ПЕЧАТЬ
        printf("LEVEL:<%2d> SIZE:<%7d> FPATH:<%-90s> NAME:<%s>\n",
            ftwbuf->level, (int)sb->st_size, fpath, fpath + ftwbuf->base);

        f_size++;
    }
    return OK;
}

// добавление файлов в список файлов
int addFile(const char *fpath, const struct stat *sb,
            int tflag, struct FTW *ftwbuf)
{
    // если текущий элемент - файл
    // добавляем его в массив
    if(tflag == FTW_F)
    {
        File* f = files[f_ind++] = createFile();

        errorPrint(setFilepath(f, fpath));
        errorPrint(openInputFile(f));
        errorPrint(readFileSize(f));
        errorPrint(readFileBuffer(f));
    }

    return OK;
}

#define MAX_FILES_IN_ARCHIVE 200

// создание списка файлов
File** initializeFileArr(char* path)
{
    // считаю количество файлов в директории и поддиректориях
    f_ind = 0;
    nftw(path, countFiles, MAX_FILES_IN_ARCHIVE, FTW_CHDIR);

    // выделяю память под массив файлов
    files = createFileArr(f_size);

    // считываю файлы
    f_ind = 0;
    nftw(path, addFile, MAX_FILES_IN_ARCHIVE, FTW_CHDIR);

    f_ind = 0;
    return files;
}

// печать файлового массива
int printFileArr(File** fa)
{
    // если списка файлов не существует 
    if(fa == NULL)
    {
        return FARR_DOESNT_EXIST;
    }

    // проходимся по каждому элементу массива и печатаем его
    // если он существует
    for(int i = 0; i < f_size; i++)
    {        
        errorPrint(printFile(fa[i]));
    }

    return OK;
}

int main(int argc, char** argv)
{
    // тестирование файловых функций
    //testFileFunc(argc, argv);

    // тестирование считывания файлов из директории
    // testFileDirRead(argc, argv);

    // переключение директории с помощью chdir для каждого элемента директории
    // int flags = FTW_CHDIR;

    // if (argc > 2 && strchr(argv[2], 'd') != NULL)
    //     flags |= FTW_DEPTH;
    // if (argc > 2 && strchr(argv[2], 'p') != NULL)
    //     flags |= FTW_PHYS;

    // if (nftw((argc < 2) ? "." : argv[1], display_info, MAX_FILES_IN_ARCHIVE, flags)
    //         == -1) {
    //     perror("nftw");
    //     exit(EXIT_FAILURE);
    // }


    File** fa = initializeFileArr(argv[1]);

    printFileArr(fa);

    deleteFileArr(fa);

    exit(EXIT_SUCCESS);
}
