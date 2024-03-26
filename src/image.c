#include "image.h"

void imageInfo(Image *img)
{
    /*
    int m_width;            // ширина картинки
    int m_height;           // высота картинки
    int m_channels;         // количество каналов
    size_t m_size;          // размер картинки в байтах
    enum AllocType m_alloc;
    */
   printf("=====================\nИнформация о картинке\n=====================\n");
   printf("width: %d\n",    img->m_width);
   printf("height: %d\n",   img->m_height);
   printf("channels: %d\n", img->m_channels);
   printf("size: %ld\n",     img->m_size);
   printf("alloc: %d\n",    img->m_alloc);
   printf("=====================\n");
}

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
        return;
    }
    INFOS("Картинка загружена\n");
}

// создание новой картинки
void imageCreate(Image *img, int width, int height, int channels, bool zeroed)
{
    img->m_size = width * height * channels;
    // если есть флаг zeroed, выделяем обнуленную память
    if(zeroed)
        img->m_data = calloc(img->m_size, sizeof(uint8_t));
    else
        img->m_data = malloc(img->m_size * sizeof(uint8_t));
    
    // самостоятельно создаем картинку
    if(img->m_data != NULL)
    {
        img->m_width = width;
        img->m_height = height;
        img->m_channels = channels;
        img->m_alloc = SELF_ALLOCATION;
    }
    else
    {
        ERRORS("Ошибка при выделении памяти под картинку\n");
        return;
    }

    INFOS("Картинка создана\n");
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
            img->m_data, 50);
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
        return;
    }
    
    INFO("Картинка сохранена: <%s>\n", filename);
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
    INFOS("Картинка удалена\n");
}

// применение фильтра собела
// @param src исходная картинка
// @param dest картинка с примененным фильтром
void imageApplySobel(const Image *src, Image *dest)
{
    // если картинка не выделена
    if(src->m_alloc == NO_ALLOCATION || src->m_data == NULL)
    {
        ERRORS("Картинка не выделена\n");
        return;
    }

    // создание новой картинки
    imageCreate(dest, src->m_width, src->m_height, src->m_channels, false);

    // ядра фильтра Собеля
    int kernelX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int kernelY[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    // генерируем новый список данных для картинки
    // проходимся по каждой строке
    for (int y = 1; y < src->m_height - 1; y++)
    {
        // проходимся по каждому столбцу
        for (int x = 1; x < src->m_width - 1; x++)
        {
            // в итоге обрабатываем каждый пиксель
            int gX = 0;
            int gY = 0;

            for (int j = -1; j <= 1; j++)
            {
                for (int i = -1; i <= 1; i++)
                {
                    int index = fmin(((y + j) * src->m_width + (x + i)) * src->m_channels, src->m_height - 1);
                    gX += src->m_data[index] * kernelX[(j + 1) * 3 + (i + 1)];
                    gY += src->m_data[index] * kernelY[(j + 1) * 3 + (i + 1)];
                }
            }
            // вычисляем значение
            double magnitude = sqrt(gX * gX + gY * gY);

            // обрещаем его, чтобы оно быол в границах [0.255]
            magnitude = fmin(255, fmax(0, magnitude));

            // присваиваем значения трем каналам (RGB)
            dest->m_data[((y - 1) * src->m_width + (x - 1)) * src->m_channels]     = (uint8_t)magnitude;
            dest->m_data[((y - 1) * src->m_width + (x - 1)) * src->m_channels + 1] = (uint8_t)magnitude;
            dest->m_data[((y - 1) * src->m_width + (x - 1)) * src->m_channels + 2] = (uint8_t)magnitude;
            // Если есть еще альфа канал
            if(src->m_channels == 4)
                dest->m_data[((y - 1) * src->m_width + (x - 1)) * src->m_channels + 3] = (uint8_t)magnitude;
        }
    }
    INFOS("Фильтер Собеля применен\n");
}

// информация для потока
typedef struct 
{
    const Image* m_src; // источник информации
    Image* m_dest;      // конечная картинка с фильтром Собеля
    int m_start_row;    // строка, с которой начинается обработка исходника в текущем потоке
    int m_end_row;      // строка, на которой кончается обработка исходника в текущем потоке
} ThreadSobelData;


// ядра фильтра Собеля
int kernelX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
int kernelY[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

// функция применения фильтра собела для потока
void *imageThreadApplySobel(void *arg)
{

    // преобразование типа
    ThreadSobelData *data = (ThreadSobelData *)arg;

    // генерируем новый список данных для картинки
    // проходимся по каждой строке
    for (int y = data->m_start_row + 1; y < data->m_end_row - 1; y++)
    {
        // проходимся по каждому столбцу
        for (int x = 1; x < data->m_src->m_width - 1; x++)
        {
            // в итоге обрабатываем каждый пиксель
            int gX = 0;
            int gY = 0;

            for (int j = -1; j <= 1; j++)
            {
                for (int i = -1; i <= 1; i++)
                {
                    int index = fmin(((y + j) * data->m_src->m_width + (x + i)) * data->m_src->m_channels,
                                     data->m_src->m_size);
                    gX += data->m_src->m_data[index] * kernelX[(j + 1) * 3 + (i + 1)];
                    gY += data->m_src->m_data[index] * kernelY[(j + 1) * 3 + (i + 1)];
                }
            }
            // вычисляем значение
            double magnitude = sqrt(gX * gX + gY * gY);

            // обрещаем его, чтобы оно быол в границах [0.255]
            magnitude = fmin(255, fmax(0, magnitude));

            // присваиваем значения трем каналам (RGB)
            data->m_dest->m_data[((y - 1) * data->m_src->m_width + (x - 1)) * data->m_src->m_channels]     = (uint8_t)magnitude;
            data->m_dest->m_data[((y - 1) * data->m_src->m_width + (x - 1)) * data->m_src->m_channels + 1] = (uint8_t)magnitude;
            data->m_dest->m_data[((y - 1) * data->m_src->m_width + (x - 1)) * data->m_src->m_channels + 2] = (uint8_t)magnitude;
            // Если есть еще альфа канал
            if(data->m_src->m_channels == 4)
                data->m_dest->m_data[((y - 1) * data->m_src->m_width + (x - 1)) * data->m_src->m_channels + 3] = (uint8_t)magnitude;
        }
    }
    INFO("Поток %ld закончил работу\n", pthread_self())
    pthread_exit(0);
}

void imageApplySobelMt(const Image *src, Image *dest, int thread_num)
{
    // если картинка не выделена
    if(src->m_alloc == NO_ALLOCATION || src->m_data == NULL)
    {
        ERRORS("Картинка не выделена\n");
        return;
    }

    // создание новой картинки
    imageCreate(dest, src->m_width, src->m_height, src->m_channels, false);

    // создаем массив потоков
    pthread_t *threads = calloc(thread_num, sizeof(pthread_t));

    // создаем массив информации под потоки
    ThreadSobelData *data = calloc(thread_num, sizeof(ThreadSobelData));

    // число строк на поток
    // если ввести число потоков больше числа строк, то получится,
    // что на каждый поток в среднем призодится по 0 строк
    // такого не должно быть
    if(thread_num > src->m_height) thread_num = src->m_height;
    int numb_of_rows_per_thread = src->m_height / thread_num;

    INFO("Число строк на поток: %d\n", numb_of_rows_per_thread);

    // формирование данных
    for(int i = 0; i < thread_num; i++)
    {
        // информация для потока
        
        data[i].m_src = src;
        data[i].m_dest = dest;
        data[i].m_start_row = fmax(i * numb_of_rows_per_thread - 1, 0);
        data[i].m_end_row =   fmin((i + 1) * numb_of_rows_per_thread + 1, src->m_height);
    }
    data[thread_num - 1].m_end_row = src->m_height;

    // запускаем потоки с определенными данными для каждого потока
    for(int i = 0; i < thread_num; i++)
    {
        INFO("Поток %d start: %d end: %d\n", i, data[i].m_start_row, data[i].m_end_row);
        
        // запуск потока
        if(pthread_create(threads + i, NULL, imageThreadApplySobel, (void *)(data + i)) != 0)
        {
            ERRORS("Ошибка создания потока\n");
            break;
        }
    }
    INFO("Все %d потоков запущены\n", thread_num);

    // ждем выполнения всех потоков
    for(int i = 0; i < thread_num; i++)
    {
        // возвращенное значение из потока
        void *ret_data;

        // статус завершения потока
        int ret_code;

        // ожидаем завершение потока
        if((ret_code = pthread_join(*(threads + i), &ret_data)) != 0)
        {
            ERROR("Поток завершился с ошибкой: %d\n", ret_code);
        }        
    }

    // удлаение массивов
    free(data);
    free(threads);
}
