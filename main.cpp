#include <iostream>
#include <omp.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION 1
#include <stb_image_write/stb_image_write.h>

#include "src/Mandelbrot.h"

int main() {
    auto result = mandelbrot_set<720, 720>(-2.0, 0.5, -1.25, 1.25, 256);

    stbi_write_bmp("wusa.bmp", 720, 720, 1, result.data());

    return 0;
}