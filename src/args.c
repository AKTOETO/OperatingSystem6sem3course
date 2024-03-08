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

    // проверка у далось ли считать строку
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
    tokens[*argc] = '\0';    

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
    size_t pos = 0;
    
    while(argv[pos])
    {
        fprintf(stdout, "Токены: [%s]\n", argv[pos]);
        pos++;
    }
}