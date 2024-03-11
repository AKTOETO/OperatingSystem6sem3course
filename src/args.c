#include "args.h"

// чтение строки с консоли
char* readLine()
{
    // строка
    char *str = NULL;

    // изначальный размер строки
    size_t len = 0;

    // размер 
    ssize_t size = 0;

    // проверка: удалось ли считать строку
    if((size = getline(&str, &len, stdin)) == -1)
    {
        free(str);

        ERRORS("Не удалось считать строку\n");
        return NULL;
    }

    // удаление ненужного символа переноса строки
    str[size - 1] = '\0';

    return str;
}

// токенизация строки
char** tokenizeString(char *str, size_t *argc)
{
    // начальный размер массива токенов
    size_t size = DEFAULT_TOKEN_COUNT;

    // номер элемента после последнего
    *argc = 0;

    // создаем массив токенов
    char **tokens = (char**)malloc(sizeof(char*) * size);

    // берем первый токен из строки
    char *token = strtok(str, TOKEN_DELIMETERS);

    // проходимся по всем строке, пока есть токены
    do
    {
        // вставляем считанных токен в массив токенов
        tokens[(*argc)++] = token;

        // проверяем, есть ли еще место в массиве токенов, 
        // если нет - добавляем
        if(*argc >= size)
        {
            // увеличиваем буфер в2 раза
            size *= 2;

            // перевыделяем память
            tokens = (char**)realloc(tokens, sizeof(char*) * size);

            // если не получилось перевыделить память, пишем об этом
            if(tokens == NULL)
            {
                ERRORS("Не получилось перевыделить память под массив токенов\n");
                return NULL;
            }

        }

        // получение следующего токена
        token = strtok(NULL, TOKEN_DELIMETERS);

    } while (token != NULL);
    
    // добавляем в последний токен NULL для определения 
    // количества элементов в массиве токенов
    tokens[*argc] = NULL;    

    INFO("Количество токенов: %ld\n", *argc);

    return tokens;
}

// печать токенов
void printTokens(char **argv)
{
    if(!argv)
    {
        ERRORS("Массив токенов не существует\n");
        return;
    }
    fprintf(stdout, "Массив токенов\n");
    size_t pos = 0;
    
    while(argv[pos])
    {
        fprintf(stdout, "Токены: %p = [%s]\n", argv + pos, argv[pos]);
        pos++;
    }
}

char **dublicateToken(size_t argc, char **argv)
{
    // проверка на отсутствие количества токенов
    if(argc <= 0 || argv == NULL)
        ERRORS("Нет токенов для копирования\n");

    // выделение памяти под новый массив токенов
    char **tokens = malloc(sizeof(char*) * (argc + 1));

    // проходимся по всему массиву переданных токенов
    for(size_t i = 0; i < argc; i++)
    {
        // копируем каждый токен
        tokens[i] = strdup(argv[i]);
    }

    // вставляем NULL в последий элемент
    tokens[argc] = NULL;

    // возврат токенов 
    return tokens;
}

void deleteDubTokens(size_t argc, char **argv)
{
    INFOS("Удаление токенов\n");
    // проходимся в цикле по каждой строке 
    for(size_t i = 0; i < argc; i++)
    {
        INFO("Удаление: [%s]\n", argv[i]);
        // удаляем строку
        free(argv[i]);
    }

    // удаляем массив
    free(argv);
}
