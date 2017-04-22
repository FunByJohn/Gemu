#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Entity {
protected:
	sf::Time animation;
	sf::Vector2f pos;
	sf::Vector2f vel;

public:
	typedef std::unique_ptr<Entity> ptr;

	virtual void tick(const sf::Time& dt, std::vector<ptr>& entities);
	virtual void render(sf::Uint8* pixels, sf::FloatRect camera) = 0;

	virtual ~Entity();
};


