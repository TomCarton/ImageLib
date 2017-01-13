// imageToolResize.c
//
// written by Thomas CARTON
//

#include <stdlib.h>

#include "../include/imaging.h"


unsigned char *Downsize(unsigned char *image, unsigned int width, unsigned int height)
{
    unsigned int line = width * 3;

    width >>= 1; height >>= 1;
    unsigned char *out = malloc(3 * width * height);

    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            unsigned int ip = y * 4 * width + 2 * x;
            unsigned char r = ((unsigned short)image[ip * 3 + 0] + image[ip * 3 + line + 0] + image[ip * 3 + 3 + 0] + image[ip * 3 + 3 + line + 0]) >> 2;
            unsigned char g = ((unsigned short)image[ip * 3 + 1] + image[ip * 3 + line + 1] + image[ip * 3 + 3 + 1] + image[ip * 3 + 3 + line + 1]) >> 2;
            unsigned char b = ((unsigned short)image[ip * 3 + 2] + image[ip * 3 + line + 2] + image[ip * 3 + 3 + 2] + image[ip * 3 + 3 + line + 2]) >> 2;

            unsigned int op = y * width + x;
            out[op * 3 + 0] = r;
            out[op * 3 + 1] = g;
            out[op * 3 + 2] = b;
        }
    }
    
    return out;
}
