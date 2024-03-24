#include "image.h"

// загрузка картинки с диска
void imageLoad(Image *img, const char *filename)
{
    if((img->m_data = stbi_load(
        filename, &img->m_width, &img->m_height, &img->m_channels, 0))
        != NULL)
    {
        img->m_size = img->m_width * img->m_height * img->m_channels;
        img->m_alloc = STB_ALLOCATED;
    }
    // если не удалось создать картинку
    else
    {
        img->m_alloc = NO_ALLOCATION;
        ERROR("Не удалось создать картинку: <%s>\n", filename);
    }
}

// создание новой картинки
void imageCreate(Image *img, int width, int height, int channels, bool zeroed)
{
    img->m_size = img->m_width * img->m_height * img->m_channels;
    // если есть флаг zeroed, выделяем обнуленную память
    if(zeroed)
        img->m_data = calloc(img->m_size, 1);
    else
        img->m_data = malloc(img->m_size);
    
    // самостоятельно создаем картинку
    if(img->m_data != NULL)
    {
        img->m_width = width;
        img->m_height = height;
        img->m_channels = channels;
        img->m_alloc = SELF_ALLOCATION;
    }
    else
        ERRORS("Ошибка при выделении памяти под картинку\n");
}

// сохранение картинки на диск
void imageSave(const Image *img, const char *filename)
{
    // если картинка не выделена
    if(img->m_alloc == NO_ALLOCATION || img->m_data == NULL)
    {
        ERRORS("Картинка не выделена\n");
        return;
    }

    // возможные окончания для jpg
    const char *jpeg_end[] = {".jpg", ".JPG", ".jpeg", ".JPEG"};

    // возможные окончания для png
    const char *png_end[] = {".png", ".PNG"};

    // если имя файла заканчивается на jpg, пишем в jpg файл
    if(strEndArrIn(filename, jpeg_end, 4))
    {
        stbi_write_jpg(
            filename, img->m_width, img->m_height, img->m_channels,
            img->m_data, 100);
    }
    // если имя файла заканчивается на png, пишем в png файл
    else if(strEndArrIn(filename, png_end, 2))
    {
        stbi_write_png(
            filename, img->m_width, img->m_height, img->m_channels,
            img->m_data, img->m_channels * img->m_width);
    }
    // если неизвестный конец файла
    else
    {
        ERROR("Неизвестное расширение файла: <%s>\n", filename);
    }
    
}

// освобождение памяти картинки
void imageFree(Image *img)
{
    // если картинка создана и информация есть
    if(img->m_alloc != NO_ALLOCATION && img->m_data != NULL)
    {
        // если картинку создали с помощью stb
        if(img->m_alloc == STB_ALLOCATED)
        {
            stbi_image_free(img->m_data);
        }
        // если картинка была создана вручную
        else
        {
            free(img->m_data);
        }
        // обнуление полей картинки
        img->m_data = NULL;
        img->m_width = 0;
        img->m_height = 0;
        img->m_size = 0;
        img->m_alloc = NO_ALLOCATION;
    }
}

// применение фильтра собела
// @param src исходная картинка
// @param dest картинка с примененным фильтром
void ImageApplySobel(const Image *src, const Image *dest)
{
}