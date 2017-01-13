// imageTypePPM.c
//
// written by Thomas CARTON
//

#include <stdio.h>

#include "../include/imaging.h"


void ImageSavePPM(char *filename, unsigned char *image, unsigned int width, unsigned int height)
{
	FILE *outputFile = fopen(filename, "w");

	// PPM header
	fprintf(outputFile, "P6 %d %d %d\n", width, height, 255);

	fwrite(image, 3, width * height, outputFile);

	fclose(outputFile);
}
