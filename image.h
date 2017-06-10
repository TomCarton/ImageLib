// image.h
//
// written by Thomas CARTON
//

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "../core/core.h"


typedef struct
{
    unsigned int width;
    unsigned int height;

    unsigned char *data;
} Image;


// resize
unsigned char *ImageDownsize(unsigned char *image, unsigned int width, unsigned int height);

// image file formats
Error ImageLoadDegas(const char *filename, Image *image);

Error ImageSaveTarga(const Image image, const char *filename);
Error ImageSavePNG(const Image image, const char *filename);
Error ImageSavePPM(const Image image, const char *filename);

#endif // __IMAGE_H__
