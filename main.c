#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// 灰度化
void grayscale(unsigned char *data, int width, int height, int channels) {
    for (int i = 0; i < width * height; i++) {
        int idx = i * channels;
        unsigned char r = data[idx];
        unsigned char g = data[idx + 1];
        unsigned char b = data[idx + 2];
        unsigned char gray = 0.299*r + 0.587*g + 0.114*b;
        data[idx] = data[idx + 1] = data[idx + 2] = gray;
    }
}

// 反色
void invert(unsigned char *data, int width, int height, int channels) {
    for (int i = 0; i < width * height * channels; i++) {
        data[i] = 255 - data[i];
    }
}

// 模糊处理（简单 3x3 box blur）
void blur(unsigned char *src, unsigned char *dest, int width, int height, int channels) {
    memcpy(dest, src, width * height * channels); // 保留边缘
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            for (int c = 0; c < channels; c++) {
                int sum = 0;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        int nx = x + dx;
                        int ny = y + dy;
                        sum += src[(ny * width + nx) * channels + c];
                    }
                }
                dest[(y * width + x) * channels + c] = sum / 9;
            }
        }
    }
}

// 水平镜像
void flip_horizontal(unsigned char *data, int width, int height, int channels) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width / 2; x++) {
            for (int c = 0; c < channels; c++) {
                int idx1 = (y * width + x) * channels + c;
                int idx2 = (y * width + (width - 1 - x)) * channels + c;
                unsigned char tmp = data[idx1];
                data[idx1] = data[idx2];
                data[idx2] = tmp;
            }
        }
    }
}

// 彩虹条纹特效
void rainbow_stripes(unsigned char *data, int width, int height, int channels) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = (y * width + x) * channels;
            if (y % 3 == 0) {
                data[idx] = 255; data[idx + 1] = 0; data[idx + 2] = 0;
            } else if (y % 3 == 1) {
                data[idx] = 0; data[idx + 1] = 255; data[idx + 2] = 0;
            } else {
                data[idx] = 0; data[idx + 1] = 0; data[idx + 2] = 255;
            }
        }
    }
}

int main() {
    int width, height, channels;
    unsigned char *img = stbi_load("input.png", &width, &height, &channels, 0);
    if (!img) {
        printf("Failed to load image.\n");
        return 1;
    }

    printf("Image loaded: %d x %d, channels: %d\n", width, height, channels);

    // 灰度化
    unsigned char *gray_img = malloc(width * height * channels);
    memcpy(gray_img, img, width * height * channels);
    grayscale(gray_img, width, height, channels);
    stbi_write_png("output_gray.png", width, height, channels, gray_img, width * channels);

    // 反色
    unsigned char *inv_img = malloc(width * height * channels);
    memcpy(inv_img, img, width * height * channels);
    invert(inv_img, width, height, channels);
    stbi_write_png("output_invert.png", width, height, channels, inv_img, width * channels);

    // 模糊
    unsigned char *blur_img = malloc(width * height * channels);
    blur(img, blur_img, width, height, channels);
    stbi_write_png("output_blur.png", width, height, channels, blur_img, width * channels);

    // 水平镜像
    unsigned char *flip_img = malloc(width * height * channels);
    memcpy(flip_img, img, width * height * channels);
    flip_horizontal(flip_img, width, height, channels);
    stbi_write_png("output_flip.png", width, height, channels, flip_img, width * channels);

    // 彩虹条纹
    unsigned char *rainbow_img = malloc(width * height * channels);
    memcpy(rainbow_img, img, width * height * channels);
    rainbow_stripes(rainbow_img, width, height, channels);
    stbi_write_png("output_rainbow.png", width, height, channels, rainbow_img, width * channels);

    // 清理内存
    stbi_image_free(img);
    free(gray_img);
    free(inv_img);
    free(blur_img);
    free(flip_img);
    free(rainbow_img);

    printf("All effects processed and images saved.\n");
    return 0;
}



