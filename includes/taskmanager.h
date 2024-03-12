#ifndef TASKMANAGER_H
#define TASKMANAGER_H


// подключение задач
#include "maintask.h"
#include "bgtask.h"
#include "fgtask.h"

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

// убить foreground задачу
int killFGTask();

// убить background задачу
int killBGTask(task_t* src, bool(*f)(task_t* src, task_t* tt));

// убить все background задачи
int killAllBGTask();

// убить и удалить все задачи
int killAndDeleteAllBGTask();

// ждем выполнения foreground задачи
int waitFGTask();

// ждем выполнения background задачи без блокировки главного процесса
int waitBGTask(); 

// завершение всех задач
int quit();

// печать информации о всех background задачах
int printBGInfo();


#endif // !TASKMANAGER_H