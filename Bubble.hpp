#pragma once

#include "Player.hpp"

class Bubble : public Entity {
public:
  const float totalAliveTime = 4.0f;

  float radius;
  float targetRadius;
  float aliveTime;
  bool taken; // ;^

	Bubble(sf::Vector2f pos, float radius);

	void tick(const sf::Time& dt, Entity::container& entities);
	void render(sf::Uint8* pixels, sf::FloatRect& camera);
};
