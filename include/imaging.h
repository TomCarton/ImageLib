// imaging.h
//
// written by Thomas CARTON
//

#ifndef __IMAGING_H__
#define __IMAGING_H__


// resize
unsigned char *ImageDownsize(unsigned char *image, unsigned int width, unsigned int height);

// image file formats
void ImageSavePNG(char *filename, unsigned char *image, unsigned int width, unsigned int height);
void ImageSavePPM(char *filename, unsigned char *image, unsigned int width, unsigned int height);
void ImageSaveTGA(char *filename, unsigned char *image, unsigned int width, unsigned int height);


#endif // __IMAGING_H__
