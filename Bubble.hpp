#pragma once

#include "Player.hpp"

class Bubble : public Entity {
public:
  float radius;
  float targetRadius;

	Bubble(sf::Vector2f pos, float radius);

	void tick(const sf::Time& dt, Entity::container& entities);
	void render(sf::Uint8* pixels, sf::FloatRect& camera);
};
