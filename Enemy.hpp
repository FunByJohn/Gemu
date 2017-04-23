#pragma once

#include "Player.hpp"

class Enemy : public Entity {
private:
  enum State {
		FOLLOW,
    BUBBLE
	};

  State state = FOLLOW;
	Player* player;
	float angle = 0;
  sf::Vector2f bubblePos;
  sf::Vector2f diePos;

	const float radius = 20.f;

public:

	Enemy(sf::Vector2f pos, Player* player);

	void tick(const sf::Time& dt, Entity::container& entities);
	void render(sf::Uint8* pixels, sf::FloatRect& camera);
};
