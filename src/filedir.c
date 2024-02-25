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
        if(!files)
            printf("there is no files %p\n", files);

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
File** fillFileArrFromDir(char* path)
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

    printf("FILES: \n");

    // проходимся по каждому элементу массива и печатаем его
    // если он существует
    for(int i = 0; i < f_size; i++)
    {        
        errorPrint(printFile(fa[i]));
    }

    return OK;
}


// создание директории, в которой должен быть файл
char* createFileDir(const char* dir)
{
    // поиск позиции имени файла
    size_t i = strlen(dir) - 1;
    for(; i > 0;)
    {
        if(dir[i] == '/') 
        {
            break;
        }
        else i--;
    }

    return strndup(dir, i+1);
}

// рекурсивное создание папок
void recursiveMkdir(char *path)
{
    char *sep = strrchr(path, '/');
    if(sep != NULL) {
        *sep = 0;
        recursiveMkdir(path);
        *sep = '/';
    }
    if(mkdir(path, 0777) == -1)
    {
        printf("error creating path: %s\n", path);
    }
    else
    {
        printf("path created: %s\n", path);
    }
}

void dearchivateFile(File *f)
{
    // полный путь до файла
    size_t full_path_size = strlen(base_folder) + f->m_path_size + 1;
    char* full_path = malloc(full_path_size + 1);
    snprintf(full_path, full_path_size, "%s/%s", base_folder, f->m_path);
    full_path[full_path_size] = 0;
    printf("full path: <%s>\n", full_path);

    // проверяем существует ли папка
    char* file_dir = createFileDir(full_path);
    struct stat st;
    if(stat(file_dir, &st) == -1)
    {
        // создаем папку, если не существует
        recursiveMkdir(file_dir);
    }
    free(file_dir);

    // создаем файл
    errorPrint(setFilepath(f, full_path));

    // создаем дескриптор
    errorPrint(openOutputFile(f));

    // печатаем буфер в файл
    errorPrint(writeFile(f));

    free(full_path);
    closeFile(f);
}

void fillDirFromFileArr(File **fa)
{
    // проходимся по всем файлам и выводим их 
    for(int i = 0; i < f_size; i++)
    {
        dearchivateFile(files[i]);
    }
}
