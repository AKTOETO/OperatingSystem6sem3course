# Операционные системы
Лабораторные работы по курсу операционные системы.

**3 курс. 6 сем.**

## Лабораторная работа №4. Многопоточность. Pthread.
Многопоточный фильтр Собела. Программа получает фотографию на вход, делится на
pthread и накладывает фильтр Собела. Каждый поток обрабатывает определенное количество строк исходного изображения. Программа фиксирует время своего выполнения.

Запустить программу с 1,2,4,8, 16 и 32 потоками, зафиксировать время выполнения для каждой реализации. Сделать выводы.

> Литература: Основы программирования в Linux. Автор: Мэтью Нейл, Стоунс
Ричард. Глава 12. Потоки POSIX.

### Варианты для всех студентов одинаковы, но желательно выполнять лабораторную работу на разных машинах.

Пример программы, реализующей фильтр Собела:
```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// Функция для применения фильтра Собела к изображению
void applySobelFilter(int image[][3], int result[][3]) {
    int gx, gy;
    // Ядро фильтра Собела по оси X
    int kernelX[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    // Ядро фильтра Собела по оси Y
    int kernelY[3][3] = {
        {1, 2, 1},
        {0, 0, 0},
        {-1, -2, -1}
    };
    for (int i = 1; i < 2; i++) {
        for (int j = 1; j < 2; j++) {
            gx = 0;
            gy = 0;
            // Применяем ядра фильтра Собела к пикселям изображения
            for (int k = -1; k <= 1; k++) {
                for (int l = -1; l <= 1; l++) {
                    gx += image[i + k][j + l] * kernelX[k + 1][l + 1];
                    gy += image[i + k][j + l] * kernelY[k + 1][l + 1];
                
                }
            }
            // Вычисляем градиент изображения
            result[i][j] = sqrt(gx * gx + gy * gy);
        }
    }
}
int main() {
    int image[3][3] = {
        {10, 20, 30},
        {40, 50, 60},
        {70, 80, 90}};
    int result[3][3];
    applySobelFilter(image, result);
    // Выводим результат применения фильтра Собела к изображению
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }
    return 0;
}
```