#include "ParticleSystem.hpp"
#include "Util.hpp"

Particle::Particle(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f acc, sf::Time liveTime, Particle::Type type, sf::Color color) {
	this->pos = pos;
	this->vel = vel;
	this->acc = acc;
	this->liveTime = liveTime;
	this->type = type;
	this->color = color;
}

Particle::Particle(std::vector<sf::Vector2f> opt, sf::Time liveTime, Particle::Type type, sf::Color color) {
	if(opt.size() >= 1) pos = opt[0];
	if(opt.size() >= 2) pos = opt[1];
	if(opt.size() >= 3) pos = opt[2];
	this->liveTime = liveTime;
	this->type = type;
	this->color = color;
}

void Particle::tick(const sf::Time& dt, Entity::container& entities) {
	elapsed += dt;
	vel += acc * dt.asSeconds();
	Entity::tick(dt, entities); // super
}

void Particle::render(sf::Uint8* pixels, sf::FloatRect camera) {
	switch(type) {
		case PIXEL:
			setPixel(pixels, camera, pos, color);
			break;
	}
}

