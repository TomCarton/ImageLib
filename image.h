// image.h
//
// written by Thomas CARTON
//

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "../core/core.h"


typedef enum
{
    kPixelFormatNone = 0,

    // half byte
    kPixelFormatPal16, // 0-15 > 3-3-3
    kPixelFormatPal16x, // 0-15 > 4-4-4

    // 1 byte
    kPixelFormatPal256, // 0-255 > 8-8-8

    // 2 bytes
    kPixelFormatRGB15, // 5-5-5
    kPixelFormatRGB16, // 5-6-5

    // 3 bytes
    kPixelFormatRGB24, // 8-8-8

    // 4 bytes
    kPixelFormatRGBA32, // 8-8-8-8

} PixelFormat;


typedef struct
{
    unsigned int width;
    unsigned int height;

    PixelFormat format;
    unsigned int *palette;

    unsigned char *data;

} Image;


unsigned int SizeForPixelFormat(const PixelFormat format);
Error ConvertPixelFormat(Image *image, const PixelFormat format);

// resize
unsigned char *ImageDownsize(unsigned char *image, unsigned int width, unsigned int height);

// image file formats
Error ImageLoadDegas(const char *filename, Image *image);

Error ImageSaveTarga(const Image image, const char *filename);

Error ImageSavePNG(const Image image, const char *filename);
Error ImageSavePPM(const Image image, const char *filename);

#endif // __IMAGE_H__
