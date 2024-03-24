#include "image.h"

// загрузка картинки с диска
void imageLoad(Image *img, const char *filename)
{
    if((img->m_data = stbi_load(
        filename, &img->m_width, &img->m_height, &img->m_channels, 0)) != NULL)
    {
        img->m_size = img->m_width * img->m_height * img->m_channels;
        img->m_alloc = STB_ALLOCATED;
    }
}

// создание новой картинки
void imageCreate(Image *img, int width, int height, int channels, bool zeroed)
{

}

// сохранение картинки на диск
void inageSave(const Image *img, const char *filename)
{
    
}

// освобождение памяти картинки
void imageFree(Image *img)
{
    
}

// применение фильтра собела
// @param src исходная картинка
// @param dest картинка с примененным фильтром
void ImageApplySobel(const Image *src, const Image *dest)
{
    
}