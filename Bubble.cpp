#include "Bubble.hpp"
#include "Util.hpp"
#include "Consts.hpp"

#include <cmath>

Bubble::Bubble(sf::Vector2f pos, float radius) {
	this->pos = pos;
  this->radius = radius;
  this->targetRadius = radius;
}

void Bubble::tick(const sf::Time& dt, Entity::container& entities) {
	Entity::tick(dt, entities);

	float fdt = dt.asSeconds();

	/*if(pos.x - radius <= 0 && vel.x < 0 ||
	   pos.x + radius >= screenWidth && vel.x > 0) angle = atan2(vel.y, -vel.x);
	if(pos.y - radius <= 0 && vel.y < 0 ||
	   pos.y + radius >= screenHeightx && vel.y > 0) angle = atan2(-vel.y, vel.x);*/

  vel.y += 1.0f * fdt;
  pos.x += vel.x * fdt;
  pos.y += vel.y * fdt;

  if (radius > targetRadius) {
    radius -= 100.0f * fdt;
    if (radius < targetRadius) radius = targetRadius;
  }

  if (pos.y > screenHeight - radius && vel.y > 0.0f) {
    pos.y = screenHeight - radius;
    vel.y *= -0.6f;
    targetRadius *= 0.9f;
  }
}

void Bubble::render(sf::Uint8* pixels, sf::FloatRect& camera) {
	const float anim = animation.asSeconds();

  int r = (int)radius + 5;
	for(int x = -r; x < r; x++) {
		for(int y = -r; y < r; y++) {
			if(((y) & 1) == 0 && hypotSqPred(x + 2.0f * cos(2.0f * 1.33f * anim), y, radius + 2.5f * sin(2.0f * 1.77f * anim))) {
        // {244, 66, 140}
				setPixel(pixels, camera, {pos.x + x, pos.y + y}, {244, 66, 140});
			}
		}
	}
}
