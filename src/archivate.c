#include "filedir.h"

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

// int display_info(const char *fpath, const struct stat *sb,
//              int tflag, struct FTW *ftwbuf)
// {
//     printf("%-3s %2d %7jd   %-90s %d %s\n",
//         (tflag == FTW_D) ?   "d"   : (tflag == FTW_DNR) ? "dnr" :
//         (tflag == FTW_DP) ?  "dp"  : (tflag == FTW_F) ?   "f" :
//         (tflag == FTW_NS) ?  "ns"  : (tflag == FTW_SL) ?  "sl" :
//         (tflag == FTW_SLN) ? "sln" : "???",
//         ftwbuf->level, (intmax_t) sb->st_size,
//         fpath, ftwbuf->base, fpath + ftwbuf->base);

//     return 0;           /* To tell nftw() to continue */
// }

void createHeader(File* out)
{
    // шапка
    // [file count]
    // [path size][path][buffer size]
    // ...
    // [path size][path][buffer size]

    // запись количества файлов
    write(out->m_id, &f_size, sizeof(f_size));

    // проходимся по всем файлам
    for(int i = 0; i < f_size; i++) 
    {
        // записываем размер пути
        write(out->m_id, &files[i]->m_path_size, sizeof(size_t));
        // записываем путь к файлу
        write(out->m_id, files[i]->m_path, files[i]->m_path_size);
        // записываем размер буфера
        write(out->m_id, &files[i]->m_size, sizeof(size_t));
    }
}

void createBody(File* out)
{
    // тело
    // [buffer]
    // ...
    // [buffer]

    // проходимся по всем файлам
    for(int i = 0; i < f_size; i++)
    {
        // записываем буфер
        write(out->m_id, files[i]->m_buffer, files[i]->m_size);
    }
}

// функция создания архива
int archivate(int argc, char** argv)
{
    // в аргументах должен быт путь к папке, которую надо заархивировать
    // и путь к файлу, в который надо выгрузить архив
    if(argc < 3)
    {
        printf("должно быть больше аргументов \n");
        return 1;
    }

    // создание массива с файлами
    initializeFileArr(argv[1]);

    // печать массива с файлами
    printFileArr(files);

    // создание выходного файла
    File* f = createFile();

    // создаю дескриптор выходного файла
    errorPrint(setFilepath(f, argv[2]));
    errorPrint(openOutputFile(f));

    // печать выходнго файла
    errorPrint(printFile(f));

    // запись шапки в буфер выходного файла
    createHeader(f);

    // выгрузка шапки
    errorPrint(writeFile(f));

    // запись тела в буфер выходного файла
    createBody(f);

    // выгрузка тела
    errorPrint(writeFile(f));

    // удаление выходного файла
    errorPrint(closeFile(f));
    deleteFile(f);

    // удаление массива с файлами
    deleteFileArr(files);

    return 0;
}

void dearchivate(int argv, char** argc)
{

}

int main(int argc, char** argv)
{
    // сохранение базовой папки
    base_folder = argv[1];

    //archivate(argc, argv);

    return 0;
}
