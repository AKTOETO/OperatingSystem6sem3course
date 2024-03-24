#ifndef IMAGE_H
#define IMAGE_H

#include "errors.h"


// перечисление для отследивания того, что создал картинку
enum AllocType
{
    NO_ALLOCATION,
    SELF_ALLOCATION,
    STB_ALLOCATED
};

// информация о картинке
typedef struct 
{
    int m_width;            // ширина картинки
    int m_height;           // высота картинки
    int m_channels;         // количество каналов
    size_t m_size;          // размер картинки в байтах
    uint8_t *m_data;        // данные картинки
    enum AllocType m_alloc; // тип выделения памяти под картиинку
} Image;

// загрузка картинки с диска
void imageLoad(Image *img, const char *filename);

// создание новой картинки
void imageCreate(Image *img, int width, int height, int channels, bool zeroed);

// сохранение картинки на диск
void imageSave(const Image *img, const char *filename);

// освобождение памяти картинки
void imageFree(Image *img);

// применение фильтра собела
// @param src исходная картинка
// @param dest картинка с примененным фильтром
void ImageApplySobel(const Image *src, const Image *dest);

#endif // !IMAGE_H