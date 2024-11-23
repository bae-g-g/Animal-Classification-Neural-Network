#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "stb_image.h"

#define input_size 4096
#define output_size 4
#define data_number 40

unsigned char *image_path[data_number];
char *img_temp[data_number];
float *img[data_number];
float target[data_number][output_size]={0};


void resize_bilinear(int index) {
    img[index] = (unsigned char* )malloc(64 * 64 * sizeof(unsigned char));
    float scale_x = (float)256 / 64.0;
    float scale_y = (float)256 / 64.0;

    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            float src_x = x * scale_x;
            float src_y = y * scale_y;

            int x1 = (int)src_x;
            int y1 = (int)src_y;
            int x2 = (x1 + 1 < 256) ? x1 + 1 : x1;
            int y2 = (y1 + 1 < 256) ? y1 + 1 : y1;

            float dx = src_x - x1;
            float dy = src_y - y1;

            float top = (1 - dx) * img_temp[index][y1 * 256 + x1] + dx * img_temp[index][y1 * 256 + x2];
            float bottom = (1 - dx) * img_temp[index][y2 * 256 + x1] + dx * img_temp[index][y2 * 256 + x2];

            float value = (1 - dy) * top + dy * bottom;
            
            img[index][y * 64 + x] = value;
        }
    }
}


void data_load(){

int width = 256;
int height =256;
int  channels;

    int index = 0;
    for(int i = 0; i<data_number/output_size; i++){

    image_path[index] = (char *)malloc(50 * sizeof(char));
    sprintf( image_path[index], "./image_data/cat_images/cat_%d.jpg", i);
    img_temp[index]= stbi_load(image_path[index], &width, &height, &channels, 1);
    resize_bilinear(index);
    index++;

    image_path[index] = (char *)malloc(50 * sizeof(char));
    sprintf( image_path[index], "./image_data/dog_images/dog_%d.jpg", i);
    img_temp[index]= stbi_load(image_path[index], &width, &height, &channels, 1);
    resize_bilinear(index);
    index++;

    image_path[index] = (char *)malloc(50 * sizeof(char));
    sprintf( image_path[index], "./image_data/tiger_images/tiger_%d.jpg", i);
    img_temp[index]= stbi_load(image_path[index], &width, &height, &channels, 1);
    resize_bilinear(index);
    index++;

    image_path[index] = (char *)malloc(50 * sizeof(char));
    sprintf( image_path[index], "./image_data/hyena_images/hyena_%d.jpg", i);
    img_temp[index]= stbi_load(image_path[index], &width, &height, &channels, 1);
    resize_bilinear(index);
    index++;

    }


    for(int i = 0 ; i<data_number; i++){
        target[i][i%output_size] = 1.0; 
    }


}


int main(){


    data_load();
    printf("%f   %f   %f  ",img[0][222],img[1][222],img[2][222]);
}