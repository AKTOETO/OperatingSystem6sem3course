#include "errors.h"
#include "file.h"

int main(int argc, char **argv)
{
    // если нет пути к файлу - выдаем ошибку
    if(argc <= 1)
    {
        ERRORS("Нужно передать путь к файлу \n");
        return -1;
    }

    // открыть файл
    File *f = createFile();

    // установить пут до файла
    setFilepath(f, argv[1]);

    // узнать размер

    // выделить память под буфер

    // считать его 

    // удалить файл
    deleteFile(f);

    INFOS("reader jopa\n");
    return 0;
}