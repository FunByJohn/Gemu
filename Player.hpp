#pragma once

#include "Entity.hpp"

class Player : public Entity {
public:
	Player(sf::Vector2f position);

	void render(sf::Uint8* pixels);
};


