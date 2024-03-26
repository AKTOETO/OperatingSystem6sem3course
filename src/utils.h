#ifndef UTILS_H
#define UTILS_H

#include "errors.h"
#include "constants.h"

extern char *g_in_file;
extern char *g_out_file;
extern int g_thread_numbers;

// проверка строки: заканчивается ли строка str на end
bool strEndIn(const char *str, const char *end);

// проверка строки: заканчивается ли строка str на один элемент из end
bool strEndArrIn(const char *str, const char *end[], size_t size);

// обработка ключей
void processARGV(int argc, char **argv);

#endif // !UTILS_H