#include "tasks.h"

// foreground задача
task_t g_fg_task = 
{
    .m_cmd = NULL,
    .m_argc = 0,
    .m_pid_id = -1,
    .m_status = FINISHED,
    .m_type = FOREGROUND
};

// все background задачи
task_t* g_bg_task = NULL;   // массив
size_t g_bg_count = 0;      // количество задач
size_t g_bg_capacity = 0;   // вместимость массива
