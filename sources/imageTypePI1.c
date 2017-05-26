// imageTypePI1.c
//
// written by Thomas CARTON
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../image.h"


Error ImageLoadPI1(const char *filename, Image **imagePtr)
{
    FILE *inputFile = fopen(filename, "r");
    if (inputFile == NULL)
        return kErrorFileOpen;

    unsigned short header;
    size_t read = fread(&header, 2, 1, inputFile);
    if (read != 1)
        return kErrorFileRead;

    if (header != 0)
        return kErrorImageFormat;

    // palette
    unsigned short paletteST[16];
    read = fread(&paletteST, 2, 16, inputFile);
    if (read != 16)
        return kErrorFileRead;

    unsigned char paletteRGB[16][3];
    for (unsigned int i = 0; i < 16; ++i)
    {
        paletteST[i] = swap16(paletteST[i]);
        unsigned short c = paletteST[i];
        c = ((c & 0x777) << 1) | ((c & 0x888) >> 3);

        paletteRGB[i][0] = ((c >> 8) & 15) * 255 / 15;
        paletteRGB[i][1] = ((c >> 4) & 15) * 255 / 15;
        paletteRGB[i][2] = (c & 15) * 255 / 15;
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
    read = fread(&buffer, 2, 16000, inputFile);
    if (read != 16000)
        return kErrorFileRead;

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
