#pragma once

#include "Player.hpp"

class Enemy : public Entity {
public:
  enum State {
		FOLLOW,
    BUBBLE
	};

  const float collideFriendTotalCooldown = 0.2f;

  State state = FOLLOW;
	Player* player;
	float angle = 0;
  sf::Vector2f bubblePos;
  sf::Vector2f diePos;
  float collideFriendCooldown;

	float radius = 20.f;

public:

	Enemy(sf::Vector2f pos, Player* player);

	void tick(const sf::Time& dt, Entity::container& entities);
	void render(sf::Uint8* pixels, sf::FloatRect& camera);
};
