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

float hypotSq(float x, float y) {
  return x * x + y * y;
}

bool hypotSqPred(float x, float y, float z) {
  return x * x + y * y <= z * z;
}

float rotateTo(float from, float to, float step) {
  float a = fmax(from, to) - fmin(from, to);
  float b = ((-M_PI + fmin(from, to)) + (M_PI - fmax(from, to))) + 2.0f * M_PI;

  if(a < b) {
    step = a < step ? a : step;
    from += from < to ? step : -step;
  } else {
    step = b < step ? b : step;
    from += from < to ? -step : step;
  }

  from = from < -M_PI ? from + 2.0f * M_PI : from;
  from = from > M_PI ? from - 2.0f * M_PI : from;

  return from;
}
