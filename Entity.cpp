#include "Entity.hpp"

void Entity::tick(const sf::Time& dt, Entity::container& entities) {
	pos += vel * dt.asSeconds();
	animation += dt;
}

Entity::~Entity() {}

