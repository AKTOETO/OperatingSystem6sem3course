#include "filedir.h"

// создание директории с файлами
FileDir* createFileDir()
{
    FileDir* fd = (FileDir*)calloc(1, sizeof(FileDir));

    // значения по умолчанию
    fd->m_files = NULL;
    fd->m_length = -1;

    return fd;
}

// удаление директории с файлами
void deleteFileDir(FileDir *f)
{
    // если файловая директория существует 
    if(f == NULL)
    {
        return FDIR_DOESNT_EXIST;
    }

    // проходимся по каждому файлу и удаляем его
    for(int i = 0; i < f->m_length; i++)
    {
        // если существует файл
        if(&f->m_files[i] == NULL)
        {
            return FILE_DOESNT_EXIST;
        }

        closeFile(&f->m_files[i]);
        deleteFile(&f->m_files[i]);
    }

    // удаляем саму директорию
    free(f);
}