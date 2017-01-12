// image.h
//
// written by Thomas CARTON
//

#ifndef __IMAGE_H_
#define __IMAGE_H_


void SaveImagePNG(char *filename, unsigned char *image, unsigned int width, unsigned int height);
void SaveImagePPM(char *filename, unsigned char *image, unsigned int width, unsigned int height);
void SaveImageTGA(char *filename, unsigned char *image, unsigned int width, unsigned int height);

unsigned char *Downsize(unsigned char *image, unsigned int width, unsigned int height);


#endif // __IMAGE_H_
