#include <algorithm>
#include <cmath>

#include "ParticleSystem.hpp"
#include "Util.hpp"

/*
 *	PARTICLE
 */

Particle::Particle(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f acc, sf::Time liveTime, Particle::Type type, sf::Color color) {
	this->pos = pos;
	this->vel = vel;
	this->acc = acc;
	this->liveTime = liveTime;
	this->type = type;
	this->color = color;

	baseRadius = 3 + rand() % 3;
}

Particle::Particle(std::vector<sf::Vector2f> opt, sf::Time liveTime, Particle::Type type, sf::Color color) {
	if(opt.size() >= 1) pos = opt[0];
	if(opt.size() >= 2) vel = opt[1];
	if(opt.size() >= 3) acc = opt[2];
	this->liveTime = liveTime;
	this->type = type;
	this->color = color;

	baseRadius = 3 + rand() % 3;
}

void Particle::tick(const sf::Time& dt, Entity::container& entities) {
	elapsed += dt;
	vel += acc * dt.asSeconds();
	Entity::tick(dt, entities); // super
}

void Particle::render(sf::Uint8* pixels, sf::FloatRect& camera) {
	float anim = elapsed.asSeconds() + pos.x;

	switch(type) {
		case PIXEL:
			setPixel(pixels, camera, pos, color);
			break;
		case CIRCLE: {
			float offsetX = 2.f * (0.2f * cos(12.f * anim));
			float offsetY = 2.f * (0.1f * sin(18.f * anim));
			float radius = baseRadius + (0.3 * sin(3.f * M_PI * anim));

			int r = (int)radius + 2;
			for(int x = -r; x < r; x++) {
				for(int y = -r; y < r; y++) {
					if(hypotSqPred(x + offsetX, y + offsetY, radius)) {
						setPixel(pixels, camera, {pos.x + x, pos.y + y}, color);
					}
				}
			}
			break;
		}
	}
}


/*
 * PARTICLESYSTEM
 */

ParticleSystem::ParticleSystem() {}


ParticleSystem* ParticleSystem::getInstance() {
	static ParticleSystem* ptr = new ParticleSystem();
	return ptr;
}

void ParticleSystem::addParticle(Particle* particle) {
	particles.emplace_back(particle);
}

void ParticleSystem::explode(sf::Vector2f pos, int count, sf::Color color) {
	for(int i = 0; i < count; i++) {
		addParticle(new Particle({pos, {-100 + rand() % 200, -100 + rand() % 200}}, sf::seconds(1.f), Particle::CIRCLE, color));
	}
}

void ParticleSystem::tick(const sf::Time& dt, Entity::container& entities) {
	for(auto& particle : particles) particle->tick(dt, entities);

	// Remove dead particles
	particles.erase(std::remove_if(particles.begin(), particles.end(),
								   [](auto& particle) { return particle->elapsed >= particle->liveTime; }),
					particles.end());
}

void ParticleSystem::render(sf::Uint8* pixels, sf::FloatRect& camera) {
	for(auto& particle : particles) particle->render(pixels, camera);
}
