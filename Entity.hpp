#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Entity {
protected:
	sf::Time animation = sf::Time::Zero;
	sf::Vector2f vel;

public:
	typedef std::unique_ptr<Entity> ptr;
	typedef std::vector<ptr> container;

	enum ID {
		PLAYER,
		BUBBLE,
		ENEMY
	};

	ID id;
	sf::Vector2f pos;
	bool dead = false;

	virtual void tick(const sf::Time& dt, container& entities);
	virtual void render(sf::Uint8* pixels, sf::FloatRect& camera) = 0;

	virtual ~Entity();
};
