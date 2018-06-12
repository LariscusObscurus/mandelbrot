#ifndef MANDELBROT_MANDELBROT_H
#define MANDELBROT_MANDELBROT_H

#include <complex>
#include <array>
#include <vector>
#include <chrono>
#include <omp.h>

template<typename T = double>
std::vector<T> linspace(T minimum, T maximum, size_t N)
{
    auto stride = (maximum - minimum) / static_cast<T>(N - 1);
    auto xs = std::vector<T>(N);

    auto val = minimum;

    for (auto &it: xs) {
        it = val;
        val += stride;
    }
    return xs;
}

inline int mandelbrot(std::complex<double> c, int iterations)
{
    auto z = c;
    for (int n = 0; n < iterations; n++) {
        if (z.real() * z.real() + z.imag() * z.imag() > 4) {
            return n;
        }
        z = z * z + c;
    }
    return 0;
}

template<size_t width, size_t height>
std::array<unsigned char, height * width>
mandelbrot_set(double xmin, double xmax, double ymin, double ymax, int iterations)
{
    auto horizontal_values = linspace(xmin, xmax, width);
    auto vertical_values = linspace(ymin, ymax, height);
    auto image = std::array<unsigned char, height * width>{};

    auto start_time =  std::chrono::steady_clock::now();

#pragma omp parallel for collapse(2) schedule(dynamic)
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            std::complex<double> c = 1i * vertical_values[row] + horizontal_values[col];
            auto required_iterations = mandelbrot(c, iterations);
            image[row * width + col] = required_iterations;
        }
    }

    auto end_time =  std::chrono::steady_clock::now();
    auto diff = end_time - start_time;
    std::cout << std::chrono::duration <double, std::milli> (diff).count() <<  " ms" << std::endl;
    return image;
}

#endif //MANDELBROT_MANDELBROT_H
