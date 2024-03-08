#ifndef INCLUDES_H
#define INCLUDES_H

#define _XOPEN_SOURCE 500 // для подключения новых функций стандарта POSIX
                          // nftw
#define _GNU_SOURCE


#define STR_SIZE 256     // максимальный размер строки 
#define TASK_ARR_SIZE 10 // начальный размер массива задач

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <ftw.h>        // nftw
#include <pwd.h>

#endif // !INCLUDES_H