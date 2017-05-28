// imageTypeDegas.c
//
// written by Thomas CARTON
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../image.h"


static Error Decompress(FILE *file, unsigned char *destination)
{
    unsigned char *dst = destination;
    unsigned char *end = dst + 32000;

    while (dst < end)
    {
        unsigned char idx;
        size_t read = fread(&idx, 1, 1, file);
        if (read != 1)
            return kErrorFileRead;

        if (idx < 128)
        {
            read = fread(dst, 1, idx + 1, file);

            dst += read;
        }
        else
        {
            unsigned char rep;
            read = fread(&rep, 1, 1, file);

            idx = 257 - idx;
            while (idx--)
            {
                *dst++ = rep;
            }
        }
    }
    
    return kErrorNone;
}

static void Interleave(unsigned short *src, unsigned short *dst)
{
    for (unsigned int y = 0; y < 200; ++y)
    {
        for (unsigned int k = 0; k < 20; ++k)
        {
            dst[y * 80 + k * 4 + 0] = src[y * 80 + k + 0];
            dst[y * 80 + k * 4 + 1] = src[y * 80 + k + 20];
            dst[y * 80 + k * 4 + 2] = src[y * 80 + k + 40];
            dst[y * 80 + k * 4 + 3] = src[y * 80 + k + 60];
        }
    }
}

Error ImageLoadDegas(const char *filename, Image **imagePtr)
{
    FILE *inputFile = fopen(filename, "r");
    if (inputFile == NULL)
        return kErrorFileOpen;

    unsigned short format;
    size_t read = fread(&format, 2, 1, inputFile);
    if (read != 1)
        return kErrorFileRead;

    format = swap16(format);

    bool rle = format & 0x8000;
    if ((format & 0x7FFF) != 0)
        return kErrorImageFormat;

    // palette
    unsigned short paletteST[16];
    read = fread(&paletteST, 2, 16, inputFile);
    if (read != 16)
        return kErrorFileRead;

    int r = 7;
    for (unsigned int i = 0; i < 16; ++i)
    {
        unsigned short c = swap16(paletteST[i]);
        if (c & 0x888)
            r = 15;

        paletteST[i] = c;
    }

    unsigned char paletteRGB[16][3];
    for (unsigned int i = 0; i < 16; ++i)
    {
        unsigned short c = paletteST[i];
        if (r == 15)
            c = ((c & 0x777) << 1) | ((c & 0x888) >> 3);

        paletteRGB[i][0] = ((c >> 8) & r) * 255 / r;
        paletteRGB[i][1] = ((c >> 4) & r) * 255 / r;
        paletteRGB[i][2] = (c & r) * 255 / r;
    }

    Image *image = malloc(sizeof(Image));
    if (image == NULL)
        return kErrorMemory;
    image->width = 320;
    image->height = 200;
    image->data = malloc(320 * 200 * 3);
    if (image->data == NULL)
        return kErrorMemory;

    unsigned short buffer[16000];

    if (rle)
    {
        unsigned char *temp = malloc(32000);
        Error err = Decompress(inputFile, temp);
        if (err != kErrorNone)
            return err;
        Interleave((unsigned short *)temp, (unsigned short *)&buffer);
    }
    else
    {
        read = fread(&buffer, 2, 16000, inputFile);
        if (read != 16000)
            return kErrorFileRead;
    }

    unsigned short *src = (unsigned short *)&buffer;
    unsigned char *data = image->data;
    for (unsigned int k = 0; k < 320 * 200 / 16; ++k)
    {
        unsigned short plan0 = swap16(*src++);
        unsigned short plan1 = swap16(*src++);
        unsigned short plan2 = swap16(*src++);
        unsigned short plan3 = swap16(*src++);

        for (unsigned int p = 0; p < 16; ++p)
        {
            unsigned int c = (plan0 & 0x8000) >> 15;
            c |= (plan1 & 0x8000) >> 14;
            c |= (plan2 & 0x8000) >> 13;
            c |= (plan3 & 0x8000) >> 12;

            *data++ = paletteRGB[c][0];
            *data++ = paletteRGB[c][1];
            *data++ = paletteRGB[c][2];

            plan0 <<= 1;
            plan1 <<= 1;
            plan2 <<= 1;
            plan3 <<= 1;
        }
    }

    fclose(inputFile);
    
    *imagePtr = image;
    
    return kErrorNone;
}
