#pragma once

#include <SFML/Graphics.hpp>

class Entity {
private:
	sf::Time animation;
	sf::Vector2f pos;
	sf::Vector2f vel;

public:
	virtual void tick(const sf::Time& dt);
	virtual void render(sf::Uint8* pixels) = 0;

	virtual ~Entity();
};

