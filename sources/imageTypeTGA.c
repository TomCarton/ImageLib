// imageTypeTGA.c
//
// written by Thomas CARTON
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/imaging.h"


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


void SaveImageTGA(char *filename, unsigned char *image, unsigned int width, unsigned int height)
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
