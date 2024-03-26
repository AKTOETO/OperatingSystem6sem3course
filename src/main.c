#include "image.h"

int main(int argc, char **argv)
{
    //char *input_file = g_in_file;
    //char *output_file = g_out_file;
//
    //// если введен входной файл
    //if(argc > 1)
    //    input_file = argv[1];
    //
    //// если введен выходной файл
    //if(argc > 2)
    //    output_file = argv[2];

    processARGV(argc, argv);

    Image img, img2;
    imageLoad(&img, g_in_file);
    if(g_use_info)
        imageInfo(&img);

    //imageApplySobel(&img, &img2);
    if(g_thread_numbers == 1)
        imageApplySobel(&img, &img2);
    else
        imageApplySobelMt(&img, &img2, g_thread_numbers);
    imageSave(&img2, g_out_file);
    
    imageFree(&img);
    imageFree(&img2);

    return 0;
}
