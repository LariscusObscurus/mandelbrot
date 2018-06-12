#include <iostream>

#include "src/Mandelbrot.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

typedef struct
{
    double r;       // a fraction between 0 and 1
    double g;       // a fraction between 0 and 1
    double b;       // a fraction between 0 and 1
} rgb;

typedef struct
{
    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
} hsv;

static rgb hsv2rgb(hsv in);

rgb hsv2rgb(hsv in)
{
    double hh, p, q, t, ff;
    long i;
    rgb out;

    if (in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if (hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long) hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch (i) {
        case 0:
            out.r = in.v;
            out.g = t;
            out.b = p;
            break;
        case 1:
            out.r = q;
            out.g = in.v;
            out.b = p;
            break;
        case 2:
            out.r = p;
            out.g = in.v;
            out.b = t;
            break;

        case 3:
            out.r = p;
            out.g = q;
            out.b = in.v;
            break;
        case 4:
            out.r = t;
            out.g = p;
            out.b = in.v;
            break;
        case 5:
        default:
            out.r = in.v;
            out.g = p;
            out.b = q;
            break;
    }
    return out;
}


double sigmoid(int value)
{
    value -= 6.;
    auto result = 1 / (std::exp(-1 * (value - 0.5)) + 1);
    return result;
}

template<size_t width, size_t height>
void ShowFractal(std::array<unsigned char, width * height> &result)
{
    auto minimum = *std::min_element(result.begin(), result.end());
    auto maximum = *std::max_element(result.begin(), result.end());

    sf::Image image;
    image.create(width, height);

#pragma omp parallel for collapse(2) schedule(dynamic)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            auto val = result.at(i * width + j);
            auto hsv_value = hsv{};
            hsv_value.h = ((360 / (maximum - minimum)) * (val - minimum) + 180) % 360;
            hsv_value.s = 1;
            hsv_value.v = sigmoid(val);
            auto rgb = hsv2rgb(hsv_value);
            image.setPixel(j, i, sf::Color(static_cast<sf::Uint8>(rgb.r * 255),
                                           static_cast<sf::Uint8>(rgb.g * 255),
                                           static_cast<sf::Uint8>(rgb.b * 255)));
        }
    }

    image.saveToFile("fractal.png");
    sf::Texture texture;
    texture.create(width, height);
    texture.update(image);

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setOrigin(0, 0);
    sprite.scale(1.f, 1.f);

    sf::RenderWindow window(sf::VideoMode(width, height), "Fractal!");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

}

int main()
{
    omp_set_num_threads(8);
    const int WIDTH = 1024;
    const int HEIGHT = 1024;
    const int ITERATIONS = 256;
    auto result = mandelbrot_set<WIDTH, HEIGHT>(-2.0, 0.5, -1.25, 1.25, ITERATIONS); //Everything
    //auto result = mandelbrot_set<WIDTH, HEIGHT>(-0.8, -0.7, 0, 0.1, ITERATIONS); //Zoomed

    ShowFractal<WIDTH, HEIGHT>(result);

    return 0;
}

