#ifndef SIMAGE_H
#define SIMAGE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
@author Tom Krajnik
A simple structure and some functions that can read and write specific bitmap formats.
Each pixel is represented by three bytes, corresponding to the red, green and blue channels.
The actual image data is arranged in a 1D array, so, e.g., the G channel of the pixel in y-th row and x-th column is at (width*y+x)*3+1
*/
typedef struct 
{
	int width;		// image width in pixels
	int height;		// image width in pixels
	int size;		// number of pixels
	int bpp;		// bytes pef pixel
	unsigned char header[138];	//this holds the BMP format header  
	unsigned char* data;		//this holds the image data
}SImage;

/*Initialises the image data structure and allocates memory*/
void init(SImage *img,int width,int height);

/*saves the image, prints image size, width and height on success*/
int saveImage(SImage *img,const char* name);

/*loads the image, prints image size, width and height on success*/
int loadImage(SImage *img,const char* name);

/*Clears the image data structure and frees memory*/
void destroy(SImage *img);
  
#endif
