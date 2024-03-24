#include "image.h"

char *g_in_file = "1.png";
char *g_out_file = "sobel.png";

int main(int argc, char **argv)
{
    char *input_file = g_in_file;
    char *output_file = g_out_file;

    // если введен входной файл
    if(argc > 1)
        input_file = argv[1];
    
    // если введен выходной файл
    if(argc > 2)
        output_file = argv[2];

    Image img, img2;
    imageLoad(&img, input_file);
    imageInfo(&img);

    imageApplySobel(&img, &img2);
    imageSave(&img2, output_file);
    
    imageFree(&img);
    imageFree(&img2);

    return 0;
}
