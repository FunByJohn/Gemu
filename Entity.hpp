#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Entity {
private:
	sf::Time animation;
	sf::Vector2f pos;
	sf::Vector2f vel;

public:
	typedef std::unique_ptr<Entity> ptr;

	virtual void tick(const sf::Time& dt);
	virtual void render(sf::Uint8* pixels) = 0;

	virtual ~Entity();
};


