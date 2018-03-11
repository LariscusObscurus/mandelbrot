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

template<size_t width, size_t height>
void ShowFractal(std::array<unsigned char, width * height> &result)
{
    sf::Image image;
    image.create(width, height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            auto val = result.at(i * width + j);
            auto hsv_value = hsv{};
            hsv_value.h = val;
            hsv_value.s = 1;
            hsv_value.v = val;
            auto rgb = hsv2rgb(hsv_value);
            image.setPixel(j, i, sf::Color(static_cast<sf::Uint8>(rgb.r * 255),
                                           static_cast<sf::Uint8>(rgb.g * 255),
                                           static_cast<sf::Uint8>(rgb.b * 255)));
        }
    }

    image.saveToFile("test.bmp");
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
    const int WIDTH = 1024;
    const int HEIGHT = 1024;
    auto result = mandelbrot_set<WIDTH, HEIGHT>(-2.0, 0.5, -1.25, 1.25, 256);

    ShowFractal<WIDTH, HEIGHT>(result);

    return 0;
}

