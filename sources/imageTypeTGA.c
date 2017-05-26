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

Error ImageSaveTGA(const char *filename, Image *image)
{
    if (image->data == NULL)
        return kErrorParameter;

    FILE *outputFile = fopen(filename, "w");
    if (outputFile == NULL)
        return kErrorFileOpen;

    TGA_HEADER header;
    memset(&header, 0, sizeof(TGA_HEADER));

    header.datatypecode = 2;
    header.width = image->width;
    header.height = image->height;
    header.bitsperpixel = 24;
    header.imagedescriptor = 0x20;
    size_t written = fwrite(&header, 1, sizeof(TGA_HEADER), outputFile);
    if (written != sizeof(TGA_HEADER))
        return kErrorFileWrite;

    unsigned char *buffer = malloc(image->width * image->height * 3);
    if (buffer == NULL)
        return kErrorMemory;

    for (unsigned int k = 0; k < image->width * image->height * 3; k += 3)
    {
        buffer[k + 0] = image->data[k + 2];
        buffer[k + 1] = image->data[k + 1];
        buffer[k + 2] = image->data[k + 0];
    }
    fwrite(buffer, 3, image->width * image->height, outputFile);
    free(buffer);
    
    fclose(outputFile);
    return kErrorNone;
}

#if 0
void ImageSaveTGA(char *filename, unsigned char *image, unsigned int width, unsigned int height)
{
	FILE *outputFile = fopen(filename, "w");
	if (outputFile == NULL)
		goto error;

	TGA_HEADER header;
	memset(&header, 0, sizeof(TGA_HEADER));

	header.datatypecode = 2;
	header.width = width;
	header.height = height;
	header.bitsperpixel = 24;
	header.imagedescriptor = 0x20;
	fwrite(&header, 1, sizeof(TGA_HEADER), outputFile);

	unsigned char *data = malloc(width * height * 3);
	if (data == NULL)
		goto error;

	for (unsigned int k = 0; k < width * height * 3; k += 3)
	{
		data[k + 0] = image[k + 2];
		data[k + 1] = image[k + 1];
		data[k + 2] = image[k + 0];
	}
	fwrite(data, 3, width * height, outputFile);
	free(data);

	fclose(outputFile);
	return;

error:
	fprintf(stderr, "Failed to write image file '%s'\n", filename);
	exit(-1);
}
#endif
