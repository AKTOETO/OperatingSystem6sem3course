#include "filedir.h"

// расположение изначальной папки
char* base_folder;
// список файлов
File** files = NULL;
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
        fa = files;
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
        //printf("LEVEL:<%2d> SIZE:<%7d> FPATH:<%-90s> NAME:<%s>\n",
        //    ftwbuf->level, (int)sb->st_size, fpath, fpath + ftwbuf->base);

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

        // изменение пути файла
        char new_path[100];
        strncpy(new_path, f->m_path + strlen(base_folder), f->m_path_size - strlen(base_folder));
        errorPrint(setFilepath(f, new_path));
    }

    return OK;
}

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
        fa = files;
    }

    // проходимся по каждому элементу массива и печатаем его
    // если он существует
    for(int i = 0; i < f_size; i++)
    {        
        errorPrint(printFile(fa[i]));
    }

    return OK;
}