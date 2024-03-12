#ifndef TASKMANAGER_H
#define TASKMANAGER_H


// подключение задач
#include "maintask.h"

// создание процесса
int createTask(size_t argc, char **argv, 
               int(*parent)(pid_t, size_t argc, char **argv),
               int(*child)(size_t argc, char **argv));

// FOREGROUND
// обработка ребенка
int fgProcChild(size_t argc, char **argv);

// обработка родителя
int fgProcParent(pid_t pid,  size_t argc, char **argv);

// BACKGROUND
// обработка ребенка
int bgProcChild(size_t argc, char **argv);

// обработка родителя
int bgProcParent(pid_t pid,  size_t argc, char **argv);

///////////////////////////

// создание fg процесса
int createForegroundTask(size_t argc, char **argv);

// создание bg процесса
int createBackgroundTask(size_t argc, char **argv);


// добавляем foreground задачу
int addForegroundTask(pid_t pid, size_t argc, char **argv);

// добавляем background задачу
int addBackgroundTask(pid_t pid, size_t argc, char **argv);

// убить все background задачи
int killAllBGTask();

// убить и удалить все задачи
int killAndDeleteAllBGTask();

// ждем выполнения foreground задачи
int waitFGTask();

// завершение всех задач
int quit();

// печать информации о всех background задачах
int printBGInfo();

// очистка памяти
int clearMem();


#endif // !TASKMANAGER_H