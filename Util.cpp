#include "Util.hpp"
#include "Consts.hpp"

#include <cmath>

void setPixel(sf::Uint8* pixels, int x, int y, sf::Color color) {
    if(x < 0 || x >= screenWidth || y < 0 || y >= screenHeight) return;

    int i = 4 * (y * screenWidth + x);
    pixels[i] = color.r;
    pixels[i+1] = color.g;
    pixels[i+2] = color.b;
    pixels[i+3] = color.a;
}

void setPixel(sf::Uint8* pixels, sf::FloatRect camera, sf::Vector2f pos, sf::Color color) {
    int x = floor(pos.x - camera.left);
    int y = floor(pos.y - camera.top);
    setPixel(pixels, x, y, color);
}


