#include "image.h"

int main(int argc, char **argv)
{
    Image img;
    imageLoad(&img, "1.png");
    imageSave(&img, "2.png");
    imageFree(&img);

    return 0;
}
