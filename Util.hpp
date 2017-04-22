#pragma once

#include <SFML/Graphics.hpp>

void setPixel(sf::Uint8* pixels, int x, int y, sf::Color color);
void setPixel(sf::Uint8* pixels, sf::FloatRect camera, sf::Vector2f pos, sf::Color color);

