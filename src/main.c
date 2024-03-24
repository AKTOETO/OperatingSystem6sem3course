#include "image.h"

int main(int argc, char **argv)
{
    Image img, img2;
    imageLoad(&img, "1.png");

    imageApplySobel(&img, &img2);
    imageSave(&img2, "sobel.png");
    
    imageFree(&img);
    imageFree(&img2);

    return 0;
}
