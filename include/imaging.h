// imaging.h
//
// written by Thomas CARTON
//

#ifndef __IMAGING_H__
#define __IMAGING_H__


// resize
unsigned char *Downsize(unsigned char *image, unsigned int width, unsigned int height);

// image file formats
void SaveImagePNG(char *filename, unsigned char *image, unsigned int width, unsigned int height);
void SaveImagePPM(char *filename, unsigned char *image, unsigned int width, unsigned int height);
void SaveImageTGA(char *filename, unsigned char *image, unsigned int width, unsigned int height);


#endif // __IMAGING_H__
