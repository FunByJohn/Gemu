#include "Enemy.hpp"
#include "Util.hpp"
#include "Consts.hpp"

#include <cmath>

Enemy::Enemy(sf::Vector2f pos, Player* player) : player(player) {
	this->pos = pos;
}

void Enemy::tick(const sf::Time& dt, Entity::container& entities) {
	Entity::tick(dt, entities);

	const float speed = 400.f;
	float fdt = dt.asSeconds();

	if(pos.x - radius <= 0 && vel.x < 0 ||
	   pos.x + radius >= screenWidth && vel.x > 0) angle = atan2(vel.y, -vel.x);
	if(pos.y - radius <= 0 && vel.y < 0 ||
	   pos.y + radius >= screenHeight && vel.y > 0) angle = atan2(-vel.y, vel.x);

	float toPlayer = atan2(player->pos.y - pos.y, player->pos.x - pos.x);

	// Hit detection
	if(hypotSqPred(player->pos.y - pos.y, player->pos.x - pos.x, player->innerRadius + radius)) {
		player->kill();
	}


	angle = rotateTo(angle, toPlayer, M_PI * fdt);
	vel.x = cos(angle) * speed;
	vel.y = sin(angle) * speed;
}

void Enemy::render(sf::Uint8* pixels, sf::FloatRect& camera) {
	const float anim = animation.asSeconds();
	const float holeRadius = 15.f + 2 * sin(anim * 10.f);
	const float holeDistance = 10.f;
	float holeAngle = angle + M_PI;

	for(int x = -radius; x < radius; x++) {
		for(int y = -radius; y < radius; y++) {
			if(hypotSqPred(x, y, radius)) {
				if(!hypotSqPred(
							x - holeDistance * cos(holeAngle),
							y - holeDistance * sin(holeAngle),
							holeRadius))
					setPixel(pixels, camera, {pos.x + x, pos.y + y}, sf::Color::Blue);
			}
		}
	}
}
