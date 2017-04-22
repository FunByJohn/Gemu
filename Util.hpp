#pragma once

#include <SFML/Graphics.hpp>

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif /* M_PI */


void setPixel(sf::Uint8* pixels, int x, int y, sf::Color color);
void setPixel(sf::Uint8* pixels, sf::FloatRect camera, sf::Vector2f pos, sf::Color color);

float hypotSq(float x, float y);
bool hypotSqPred(float x, float y, float z);
float rotateTo(float from, float to, float step);
