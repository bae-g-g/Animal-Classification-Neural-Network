#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int width, height, channels;
    const char *image_path = "Image0.png";  // 입력 이미지 경로
    width = 256;
    height = 256;

    // 이미지 불러오기 (그레이스케일)
    unsigned char *img = stbi_load(image_path, &width, &height, &channels, 1);
    if (img == NULL) {
        printf("이미지를 불러오지 못했습니다: %s\n", image_path);
        return 1;
    }

    // 텍스트 파일 열기
    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("텍스트 파일을 열 수 없습니다.\n");
        stbi_image_free(img);
        return 1;
    }

    // 픽셀 데이터를 텍스트 파일에 저장
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pixel_index = y * width + x;
            fprintf(file, "%d ", img[pixel_index]/2);  // 픽셀 값 저장
        }
        fprintf(file, "\n");  // 줄바꿈
    }
    
    printf("이미지 데이터를 output.txt에 저장했습니다.\n");

    // 파일 닫기 및 메모리 해제
    fclose(file);
    stbi_image_free(img);

    return 0;
}
