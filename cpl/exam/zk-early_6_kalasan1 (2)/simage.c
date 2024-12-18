#include "simage.h"

static unsigned char sth[] = {66,77,54,16,14,0,0,0,0,0,54,0,0,0,40,0,0,0,128,2,0,0,224,1,0,0,1,0,24,0,0,0,0,0,0,16,14,0,18,11,0,0,18,11,0,0,0,0,0,0,0,0,0,0};

void init(SImage *img,int wi,int he)
{
	img->width =  wi;
	img->height = he;
	img->bpp= 3;
	img->size = img->bpp*img->width*img->height;
	img->data = (unsigned char*)calloc(img->size,sizeof(unsigned char));
	memset(img->header,0,138);
	memcpy(img->header,sth,54);
	img->header[18] = img->width%256;
	img->header[19] = img->width/256;
	img->header[22] = img->height%256;
	img->header[23] = img->height/256;
	img->header[2] = (img->size+122)%256;
	img->header[3] = ((img->size+122)/256)%256;
	img->header[4] = ((img->size+122)/256/256)%256;
	img->header[5] = ((img->size+122)/256/256)/256;
	img->header[34] = (img->size)%256;
	img->header[35] = ((img->size)/256)%256;
	img->header[36] = ((img->size)/256/256)%256;
	img->header[37] = ((img->size)/256/256)/256;
	img->header[10] = 122;
}

void destroy(SImage *img)
{
	free(img->data);
	memset(img->header,0,138);
	img->width =  img->height = img->bpp= img->size = 0;
	img->data = NULL; 
}

void swap(SImage *img)
{
  for (int j = 0;j<img->height;j++){
	  for (int i = 0;i<img->width;i++){
		  char a = img->data[(img->width*j+i)*3]; 
		  img->data[(img->width*j+i)*3] = img->data[(img->width*j+i)*3+2];
		  img->data[(img->width*j+i)*3+2] = a; 
	  }
  }
}

int saveImage(SImage *img,const char* inName)
{
	FILE* file = fopen(inName,"wb+");
	if (file == NULL){
		fprintf(stderr,"Could not save to %s\n",inName);
	       	return -1;
	}
	fwrite(img->header,122,1,file);
	fwrite(img->data,img->size,1,file);
	printf("Saved %i pixels in %ix%i bitmap.\n",img->size,img->width,img->height);
	fclose(file);
	return 0;
}

int loadImage(SImage *img,const char* inName)
{
	init(img,10,10);
	int retval = -1;
	FILE* file = fopen(inName,"rb");
	retval = 0;
	if (file!=NULL)
	{
		if (fread(img->data,54,1,file)!=1){
		       	fprintf(stderr,"Warning! Image header could not be read.\n");
			retval = -1;
		}
		img->bpp = 3;
		memcpy(img->header,img->data,54);
		int headerWidth = img->header[18]+img->header[19]*256;
		int headerHeight = img->header[22]+img->header[23]*256;
		if (headerWidth != img->width || headerHeight != img->height){
			img->height = headerHeight;
			img->width = headerWidth;
			img->size = img->height*img->width*img->bpp;
			free(img->data);
			img->data = (unsigned char*)calloc(img->size,sizeof(unsigned char));
		}
		int offset = img->header[10]+img->header[11]*256;
		if (offset-54 > 0 && fread(img->data,offset-54,1,file)!=1){ fprintf(stderr,"Warning! Image header could not be read.\n");retval = -1;}
		if (fread(img->data,img->size,1,file)!=1){
		       	fprintf(stderr,"Warning! Image data could not be read.\n");;
			retval = -1;
		}
		fclose(file);
		printf("Image with %i pixels arranged as %i x %i. Header size is %i\n",img->size,img->height,img->width,offset);
	}
	return retval;
}

