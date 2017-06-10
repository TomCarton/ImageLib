// imageTypeTGA.c
//
// written by Thomas CARTON
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../image.h"


#pragma pack(push)
#pragma pack(1)

typedef struct
{
	char  idlength;
	char  colourmaptype;
	char  datatypecode;

	short colourmaporigin;
	short colourmaplength;
	char  colourmapdepth;

	short x_origin;
	short y_origin;
	short width;
	short height;
	char  bitsperpixel;
	char  imagedescriptor;
} TGA_HEADER;

#pragma pack(pop)

Error ImageSaveTarga(const Image image, const char *filename)
{
    if (image.data == NULL)
        return kErrorParameter;

    FILE *outputFile = fopen(filename, "w");
    if (outputFile == NULL)
        return kErrorFileOpen;

    TGA_HEADER header;
    memset(&header, 0, sizeof(TGA_HEADER));

    header.datatypecode = 2;
    header.width = image.width;
    header.height = image.height;
    header.bitsperpixel = 24;
    header.imagedescriptor = 0x20;
    size_t written = fwrite(&header, 1, sizeof(TGA_HEADER), outputFile);
    if (written != sizeof(TGA_HEADER))
        return kErrorFileWrite;

    unsigned char *buffer = malloc(image.width * image.height * 3);
    if (buffer == NULL)
        return kErrorMemory;

    for (unsigned int k = 0; k < image.width * image.height * 3; k += 3)
    {
        buffer[k + 0] = image.data[k + 2];
        buffer[k + 1] = image.data[k + 1];
        buffer[k + 2] = image.data[k + 0];
    }
    fwrite(buffer, 3, image.width * image.height, outputFile);
    free(buffer);
    
    fclose(outputFile);
    return kErrorNone;
}
