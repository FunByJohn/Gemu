#include "Enemy.hpp"
#include "Util.hpp"
#include "Consts.hpp"
#include "Bubble.hpp"

#include <cmath>

Enemy::Enemy(sf::Vector2f pos, Player* player) : player(player) {
	this->pos = pos;
	id = Entity::ENEMY;
}

void Enemy::tick(const sf::Time& dt, Entity::container& entities) {
	Entity::tick(dt, entities);

  float anim = animation.asSeconds();
	const float speed = 200.f + 170.0f * sin(2.0f * M_PI * anim); // 300.0f
	float fdt = dt.asSeconds();

  switch(state) {
    case FOLLOW:
    {
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

      for (auto& entity : entities) {
        if (entity->id == Entity::BUBBLE) {
          Bubble* bubble = (Bubble*)entity.get();
          if (hypotSqPred(pos.x - entity->pos.x, pos.y - entity->pos.y, radius + bubble->radius)) {
            state = BUBBLE;
            vel.x = 0.0f;
            vel.y = 0.0f;
            bubblePos = entity->pos;
            diePos = pos;
            animation = sf::Time::Zero;
            bubble->aliveTime = 0.0f; // sin(pi * (1/2)x)
            bubble->targetRadius *= 1.5f;
            bubble->taken = true;
          }
        }
      }

      break;
    }
    case BUBBLE:
    {
      float anim2 = 5.0f * animation.asSeconds();
      float t = sin(M_PI * (1.0f / 2.0f) * anim2); //anim * anim * anim;
      if (anim2 > 1.0f) t = 1.0f;
      pos.x = diePos.x + (bubblePos.x - diePos.x) * t;
      pos.y = diePos.y + (bubblePos.y - diePos.y) * t;
      radius -= 10.0f * fdt;
      if (radius < 0.0f) {
        radius = 0.0f;
        dead = true;
      }
      break;
    }
  }
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
