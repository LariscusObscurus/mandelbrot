#ifndef MANDELBROT_MANDELBROT_H
#define MANDELBROT_MANDELBROT_H

#include <complex>
#include <array>
#include <vector>

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
        if (std::abs(z) > 2) {
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

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            std::complex<double> c = 1i * vertical_values[row] + horizontal_values[col];
            auto required_iterations = mandelbrot(c, iterations);
            try {
                image.at(row * width + col) = required_iterations;
            } catch (std::out_of_range) {
               volatile int a = 0;
            }

        }
    }
    return image;
}

#endif //MANDELBROT_MANDELBROT_H
