#include "filedir.h"

int printFileDir(FileDir *fd)
{
    // если файловая директория не существует 
    if(fd == NULL)
    {
        return FDIR_DOESNT_EXIST;
    }

    printf("\nDIR INFO\n");

    // печать пути
    printf("dirpath : <%s>\n", fd->m_path);
    printf("filenumb: <%d>\n", fd->m_length);

    // если не существует массив файлов
    if(fd->m_files == NULL)
    {
        return FDIR_FILES_DOESNT_EXIST;
    }

    // печать информации о файлах
    for(int i = 0; i < fd->m_length; i++)
    {
        // если файл есть
        //if(fd->m_files[i] != NULL)
            printFile(fd->m_files[i]);
    }

    return OK;
}

// создание директории с файлами
FileDir* createFileDir()
{
    FileDir* fd = (FileDir*)calloc(1, sizeof(FileDir));

    // значения по умолчанию
    fd->m_files = NULL;
    fd->m_length = 0;
    fd->m_path = "";

    return fd;
}

// удаление директории с файлами
int deleteFileDir(FileDir *fd)
{
    // если файловая директория не существует 
    if(fd == NULL)
    {
        return FDIR_DOESNT_EXIST;
    }

    // если существует массив файлов
    if(fd->m_files != NULL)
    {
        // проходимся по каждому файлу и удаляем его
        for(int i = 0; i < fd->m_length; i++)
        {
            // если существует файл
            if(fd->m_files[i] != NULL)
            {
                closeFile(fd->m_files[i]);
                deleteFile(fd->m_files[i]);
            }
        }
    }

    // удаляем массив файлов
    free(fd->m_files);

    // удаляем саму директорию
    free(fd);

    return OK;
}

int setPath(FileDir *f, const char *path)
{
    // если файловая директория не существует 
    if(f == NULL)
    {
        return FDIR_DOESNT_EXIST;
    }

    // установка пути
    f->m_path = path;

    return OK;
}

int countFilesInDirectory(FileDir *fd)
{
    // если файловая директория не существует 
    if(fd == NULL)
    {
        return FDIR_DOESNT_EXIST;
    }

    // поток каталога
    DIR* dir;

    // элемент каталога
    struct dirent* entry;

    // сбор информации о элементе
    struct stat statbuf;

    // если нет пути в директории
    if(strlen(fd->m_path) == 0)
    {
        return FDIR_PATH_DOESNT_EXIST;
    }

    // пытаемся открыть каталог
    if((dir = opendir(fd->m_path)) == NULL)
    {
        return FDIR_COULDNT_CREATE_DIR;
    }

    // пытаемся сменить директорию
    if(chdir(fd->m_path) == -1)
    {
        return FDIR_COULDNT_CHANGE_DIR;
    }

    // проходимся по всей директории
    while((entry = readdir(dir)) != NULL)
    {
        // получение справочной информации о текущем элементе каталога
        lstat(entry->d_name, &statbuf);

        // если же текущий элемент - файл
        if(S_ISREG(statbuf.st_mode))
        {
            // увеличиваем количество файлов
            fd->m_length++;
        }
    }

    // пытаемся вернуться в директорию выше
    if(chdir("..") == -1)
    {
        return FDIR_COULDNT_CHANGE_DIR;
    }

    // пытаемся закрыть поток каталога
    if(closedir(dir) == -1)
    {
        return FDIR_COULDNT_CLOSE_DIR;
    }

    return OK;
}

int allocateFileArray(FileDir *fd)
{   
    // если файловая директория не существует 
    if(fd == NULL)
    {
        return FDIR_DOESNT_EXIST;
    }

    // если количество файлов 0
    if(fd->m_length == 0)
    {
        return FDIR_FILE_COUNT_IS_0;
    }

    fd->m_files = (File**)calloc(fd->m_length, sizeof(File*));

    // если не получилось выделить память
    if(fd->m_files == NULL)
    {
        return FDIR_COULDNT_CREATE_FILES;
    }

    return OK;
}

int readFilesInDir(FileDir *fd)
{
    // если файловая директория не существует 
    if(fd == NULL)
    {
        return FDIR_DOESNT_EXIST;
    }

    // если не существует массив файлов
    if(fd->m_files == NULL)
    {
        return FDIR_FILES_DOESNT_EXIST;
    }

    // поток каталога
    DIR* dir;

    // элемент каталога
    struct dirent* entry;

    // сбор информации о элементе
    struct stat statbuf;

    // текущий файл
    int cur_file = 0;

    // если нет пути в директории
    if(strlen(fd->m_path) == 0)
    {
        return FDIR_PATH_DOESNT_EXIST;
    }

    // пытаемся открыть каталог
    if((dir = opendir(fd->m_path)) == NULL)
    {
        return FDIR_COULDNT_CREATE_DIR;
    }

    // пытаемся сменить директорию
    if(chdir(fd->m_path) == -1)
    {
        return FDIR_COULDNT_CHANGE_DIR;
    }

    // проходимся по всей директории
    while((entry = readdir(dir)) != NULL)
    {
        // получение справочной информации о текущем элементе каталога
        lstat(entry->d_name, &statbuf);

        // если же текущий элемент - файл
        if(S_ISREG(statbuf.st_mode))
        {
            // чтение файла
            File* f = fd->m_files[cur_file] = createFile();

            // создание пути к файлу
            int size = (strlen(fd->m_path) + strlen(entry->d_name) + 2);
            char* full_path = (char*)calloc(size, sizeof(char));
            snprintf(full_path, size, "%s/%s", fd->m_path, entry->d_name);

            errorPrint(setFilepath(f, full_path));
            errorPrint(openInputFile(f));
            errorPrint(readFileSize(f));
            errorPrint(readFileBuffer(f));

            // удаление пути 
            free(full_path);

            // переход к следующему файлу
            cur_file++;
        }
    }

    // пытаемся вернуться в директорию выше
    if(chdir("..") == -1)
    {
        return FDIR_COULDNT_CHANGE_DIR;
    }

    // пытаемся закрыть поток каталога
    if(closedir(dir) == -1)
    {
        return FDIR_COULDNT_CLOSE_DIR;
    }

    return OK;
}
