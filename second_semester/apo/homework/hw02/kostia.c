#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    unsigned char red,green,blue;
} PPMPixel;

typedef struct {
    int w, h;
    PPMPixel *data;
} PPMImage;

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "Wrong arguments given\n");
        return 1;
    }

    char buff[16];
    PPMImage *img;
    PPMImage *convimg;
    FILE *fp;
    int rgbscanval;
    fp = fopen(argv[1], "rb");
    if (!fp) {
        fprintf(stderr, "Unable to open img file '%s'\n", argv[1]);
        return 1;
    }

    if (!fgets(buff, sizeof(buff), fp)) {
        return 1;
    }

    if (buff[0] != 'P' || buff[1] != '6') {
         fprintf(stderr, "Invalid P6 format\n");
         return 1;
    }
    img = (PPMImage *)malloc(sizeof(PPMImage));
    if (!img) {
         fprintf(stderr, "Malloc fail\n");
         return 1;
    }

    if (fscanf(fp, "%d %d", &img->w, &img->h) != 2) {
         fprintf(stderr, "Invalid image size\n");
         return 1;
    }
    if (fscanf(fp, "%d", &rgbscanval) != 1) {
         fprintf(stderr, "Invalid max rgb value");
         return 1;
    }

    while (fgetc(fp) != '\n');

    img->data = (PPMPixel*)malloc(img->w * img->h * sizeof(PPMPixel));

    if (!img) {
         fprintf(stderr, "Malloc fail\n");
         return 1;
    }
    if (fread(img->data, 3 * img->w, img->h, fp) != img->h) {
         fprintf(stderr, "Error loading img file '%s'\n", argv[1]);
         return 1;
    }

    fclose(fp);

    convimg = (PPMImage *)malloc(sizeof(PPMImage));
    convimg->data = (PPMPixel*)malloc(img->w * img->h * sizeof(PPMPixel));

    int histogram[6] = {0};

    for(int j = 0; j < img->w; ++j) {
        convimg->data[j].red = img->data[j].red; 
        convimg->data[j].green = img->data[j].green; 
        convimg->data[j].blue = img->data[j].blue;  
        int hstval = round(0.2126 * convimg->data[j].red  + 0.7152 * convimg->data[j].green + 0.0722 * convimg->data[j].blue) / 51;
        ++histogram[hstval];
    }

    for(int i = 1; i < img->h - 1; ++i) {
        convimg->data[i * img->w].red = img->data[i * img->w].red; 
        convimg->data[i * img->w].green = img->data[i * img->w].green; 
        convimg->data[i * img->w].blue = img->data[i * img->w].blue;  
        int hstval = round(0.2126 * convimg->data[i * img->w].red  + 0.7152 * convimg->data[i * img->w].green + 0.0722 * convimg->data[i * img->w].blue) / 51;
        ++histogram[hstval];
        for(int j = 1; j < img->w - 1; ++j) {
            //printf("Number %d %d %d\n", img->data[i * img->w + j].red, img->data[i * img->w + j].green, img->data[i * img->w + j].blue);
            int red = (int)img->data[(i - 1) * img->w + j].red * (-1) + (int)img->data[(i + 1) * img->w + j].red * (-1) + (int)img->data[i * img->w + j - 1].red * (-1) + (int)img->data[i * img->w + j + 1].red * (-1) + (int)img->data[i * img->w + j].red * 5;
            if(red < 0) {
                convimg->data[i * img->w + j].red = 0;
            } else if(red > 255) {
                convimg->data[i * img->w + j].red = 255;
            } else {
                convimg->data[i * img->w + j].red = (unsigned char)red;
            } 
            
            int green = (int)img->data[(i - 1) * img->w + j].green * (-1) + (int)img->data[(i + 1) * img->w + j].green * (-1) + (int)img->data[i * img->w + j - 1].green * (-1) + (int)img->data[i * img->w + j + 1].green * (-1) + (int)img->data[i * img->w + j].green * 5;
            if(green < 0) {
                convimg->data[i * img->w + j].green = 0;
            } else if(green > 255) {
                convimg->data[i * img->w + j].green = 255;
            } else {
                convimg->data[i * img->w + j].green = (unsigned char)green;
            }

            int blue = (int)img->data[(i - 1) * img->w + j].blue * (-1) + (int)img->data[(i + 1) * img->w + j].blue * (-1) + (int)img->data[i * img->w + j - 1].blue * (-1) + (int)img->data[i * img->w + j + 1].blue * (-1) + (int)img->data[i * img->w + j].blue * 5;
            if(blue < 0) {
                convimg->data[i * img->w + j].blue = 0;
            } else if(blue > 255) {
                convimg->data[i * img->w + j].blue = 255;
            } else {
                convimg->data[i * img->w + j].blue = blue;
            }

            int hstvalin = round(0.2126 * convimg->data[i * img->w + j].red  + 0.7152 * convimg->data[i * img->w + j].green + 0.0722 * convimg->data[i * img->w + j].blue) / 51;
            ++histogram[hstvalin];
        }
        convimg->data[i * img->w + img->w - 1].red = img->data[i * img->w + img->w - 1].red; 
        convimg->data[i * img->w + img->w - 1].green = img->data[i * img->w + img->w - 1].green; 
        convimg->data[i * img->w + img->w - 1].blue = img->data[i * img->w + img->w - 1].blue;  
        ++histogram[(int)round(0.2126 * convimg->data[i * img->w + img->w - 1].red  + 0.7152 * convimg->data[i * img->w + img->w - 1].green + 0.0722 * convimg->data[i * img->w + img->w - 1].blue) / 51];
    }

    for(int j = 0; j < img->w; ++j) {
        convimg->data[(img->h - 1) * img->w + j].red = img->data[(img->h - 1) * img->w + j].red; 
        convimg->data[(img->h - 1) * img->w + j].green = img->data[(img->h - 1) * img->w + j].green; 
        convimg->data[(img->h - 1) * img->w + j].blue = img->data[(img->h - 1) * img->w + j].blue;  
        int hstval = round(0.2126 * convimg->data[(img->h - 1) * img->w + j].red  + 0.7152 * convimg->data[(img->h - 1) * img->w + j].green + 0.0722 * convimg->data[(img->h - 1) * img->w + j].blue) / 51;
        ++histogram[hstval];
    }
    
    FILE *txtp = fopen("output.txt", "w");
    histogram[4] += histogram[5];
    if(!txtp) {
        fprintf(stderr, "Unable to open text file\n");
    }
    for(int i = 0; i < 4; ++i) {
        fprintf(txtp, "%d ", histogram[i]);
    }
    fprintf(txtp, "%d", histogram[4]);
    fclose(txtp);

    FILE *imgp = fopen("output.ppm", "wb");
    if(!imgp) {
        fprintf(stderr, "Unable to open ppm file\n");
    }
    fprintf(imgp, "P6\n%d %d\n255\n", img->w, img->h);
    fwrite(convimg->data, sizeof(PPMPixel), img->w * img->h, imgp);
    fclose(imgp);
    free(img->data);
    free(img);
    free(convimg->data);
    free(convimg);

    //for(int i=0; i<img->w * img->h; i++)
        //printf("%d %d %d\n", convimg->data[i].red, convimg->data[i].green, convimg->data[i].blue);
    
    return 0; 
}