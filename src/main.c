#include "errors.h"

#define DEFAULT_TOKEN_COUNT 5
#define TOKEN_DELIMETERS " \n\0"

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
char** tokenizeString(char *str)
{
    // начальный размер массива токенов
    size_t size = DEFAULT_TOKEN_COUNT;

    // номер элемента после последнего
    size_t pos = 0;

    // создаем массив токенов
    char **tokens = (char**)malloc(sizeof(char*) * size);

    // берем первый токен из строки
    char *token = strtok(str, TOKEN_DELIMETERS);

    // проходимся по всем строке, пока есть токены
    do
    {
        // вставляем считанных токен в массив токенов
        tokens[pos++] = token;

        // проверяем, есть ли еще место в массиве токенов, 
        // если нет - добавляем
        if(pos >= size)
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
    tokens[pos] = '\0';    

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
    
    // первый токен
    //char *token = argv[pos];
    while(argv[pos])
    {
        INFO("\tТокены: [%s]\n", argv[pos++]);
    }
}

int main()
{
    char *str = NULL;
    char **argv = NULL;

    while(true)
    {
        // считывание строки
        str = readLine();
        if(str == NULL)
        {
            return 1;
        }
        INFO("%s\n", str);

        // получение токенов
        argv = tokenizeString(str);
        if(argv == NULL)
        {
            free(str);
            return 1;
        }

        // печать токенов
        printTokens(argv);

        if(strcmp(str, "out") == 0)
            break;

        free(str); 
        free(argv);
    }


    // pid_t pid = fork();
    
    // if(pid == 0)
    // {
    //     INFOS("imma child\n");
    //     execlp("ls", "ls", "-la", NULL);
    //     exit(1);
    // }
    // else
    // {
    //     INFOS("waiting child\n");
    //     waitpid(pid, NULL, 0);
    // }
    if(str) free(str);
    if(argv) free(argv);
    return 0;
}
