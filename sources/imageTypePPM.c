// imageTypePPM.c
//
// written by Thomas CARTON
//

#include <stdio.h>

#include "../image.h"

Error ImageSavePPM(const Image image, const char *filename)
{
    if (image.data == NULL)
        return kErrorParameter;

	FILE *outputFile = fopen(filename, "w");
    if (outputFile == NULL)
        return kErrorFileCreate;

    Error result = kErrorNone;

	// PPM header
	int written = fprintf(outputFile, "P6 %d %d %d\n", image.width, image.height, 255);
    if (written < 0)
    {
        result = kErrorFileWrite;
        goto end;
    }

	written = fwrite(image.data, 3, image.width * image.height, outputFile);
    if (written != image.width * image.height * 3)
    {
        result = kErrorFileWrite;
    }

end:
	fclose(outputFile);

    return result;
}
