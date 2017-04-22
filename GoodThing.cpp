#include "GoodThing.hpp"
#include "Util.hpp"
#include "Matrix.hpp"
#include "ParticleSystem.hpp"

#include <cmath>

GoodThing::GoodThing(sf::Vector2f pos, Player* player) : player(player) {
	this->pos = pos;
}


void GoodThing::tick(const sf::Time& dt, Entity::container& entities) {
	Entity::tick(dt, entities);

	ParticleSystem::getInstance()->addParticle(new Particle(
				{pos, {-100.f + (rand() % 200), -100.f + (rand() % 200)}},
				sf::seconds(1.5f), Particle::PIXEL, sf::Color::Blue));
}

void GoodThing::render(sf::Uint8* pixels, sf::FloatRect& camera) {
	float anim = -animation.asSeconds() * 3.f;

	Matrix<2> rot({{1, -sin(anim)},
				   {sin(anim), 1}});
				//{sin(anim), cos(anim)}});

	int r = 10;
	int bigR = r * r;
	for(int x = -bigR; x < bigR; x++) {
		for(int y = -bigR; y < bigR; y++) {
			//std::array<float, 2> npos = rot.apply({x, y});
			std::array<float, 2> npos = {x, y};

			if(hypotSqPred(npos[0], npos[0], r) && hypotSqPred(npos[1], npos[1], r))
				setPixel(pixels, camera, {pos.x + x, pos.y + y}, sf::Color::Blue);
		}
	}
}

