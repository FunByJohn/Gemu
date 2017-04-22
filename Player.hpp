#pragma once

#include "Entity.hpp"

class Player : public Entity {
public:
	enum State {
		FREE,
		DRAWING,
		FOLLOWING
	};

	State state = FREE;

	Player(sf::Vector2f position);

	void tick(const sf::Time& dt, std::vector<Entity::ptr>& entities);
	void render(sf::Uint8* pixels, sf::FloatRect camera);
};


