#pragma once

#include "Player.hpp"

class Enemy : public Entity {
private:
	Player* player;
	float angle = 0;

public:
	Enemy(sf::Vector2f pos, Player* player);

	void tick(const sf::Time& dt, Entity::container& entities);
	void render(sf::Uint8* pixels, sf::FloatRect& camera);
};
