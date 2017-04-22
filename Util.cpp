#include "Util.hpp"
#include "Consts.hpp"

void setPixel(sf::Uint8* pixels, int x, int y, sf::Color color) {
    if(x < 0 || x >= screenWidth || y < 0 || y >= screenHeight) return;

    int i = 4 * (y * screenWidth + x);
    pixels[i] = color.r;
    pixels[i+1] = color.g;
    pixels[i+2] = color.b;
    pixels[i+3] = color.a;
}

float hypotSq(float x, float y) {
  return x * x + y * y;
}

bool hypotSqPred(float x, float y, float z) {
  return x * x + y * y <= z * z;
}
