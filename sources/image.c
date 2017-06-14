//  image.c
//
//  written by Thomas CARTON
//

#import <stdlib.h>

#import "../../core/core.h"

#import "../image.h"


unsigned int SizeForPixelFormat(const PixelFormat format)
{
    switch (format)
    {
        case kPixelFormatPal256:
            return 1;

        case kPixelFormatRGB15:
        case kPixelFormatRGB16:
            return 2;

        case kPixelFormatRGB24:
            return 3;

        case kPixelFormatRGBA32:
            return 4;

        case kPixelFormatNone:
        default:
            return 0;
    }
}

Error ConvertPixelFormat(Image *image, const PixelFormat format)
{
    if (image->data == NULL)
        return kErrorParameter;

    if (image->format == format)
        return kErrorNothing;

    unsigned int size = SizeForPixelFormat(format);
    if (size == 0)
        return kErrorFormat;

    unsigned char *buffer = (unsigned char *)malloc(image->width * image->height * size);
    if (buffer == NULL)
        return kErrorMemory;

    unsigned char *src = (unsigned char *)image->data;
    unsigned char *dst = buffer;

    free(image->data);
    image->data = buffer;

    return kErrorNone;
}
