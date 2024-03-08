#ifndef STRINGS_H
#define STRINGS_H

#include "errors.h"

#define DEFAULT_TOKEN_COUNT 5
#define TOKEN_DELIMETERS " \n\0"

// чтение строки с консоли
char* readLine();

// токенизация строки
char** tokenizeString(char *str);

// печать токенов
void printTokens(char **argv);

#endif // !STRINGS_H