#ifndef SIGNALS_H
#define SIGNALS_H

#include "taskmanager.h"

// уничтожение background задач
void killAllBGTaskSignal(int sig);

// обработка zombie задач
void childKill(int sig);

// инициализация сигналов
void sigInit();

#endif // !SIGNALS_H