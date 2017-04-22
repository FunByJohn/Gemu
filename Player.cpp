#include <cmath>

#include "Player.hpp"
#include "Util.hpp"
#include "Consts.hpp"

Player::Player(sf::Vector2f position) {
	pos = position;
  vel.x = 50.0f;
}

void Player::tick(const sf::Time& dt, std::vector<Entity::ptr>& entities) {
  float fdt = dt.asSeconds();
  animation += dt;
  vel.y += 1000.0f * fdt;
  pos.x += vel.x * fdt;
  pos.y += vel.y * fdt;
  if (pos.y > screenHeight && vel.y > 0.0f) {
    vel.y = -fabs(vel.y) * 1.01f;
  }
  if (pos.x > screenWidth && vel.x > 0.0f) {
    vel.x = -fabs(vel.x);
  }
  if (pos.x < 0.0f && vel.x < 0.0f) {
    vel.x = -fabs(vel.x);
  }
}

void Player::render(sf::Uint8* pixels, sf::FloatRect camera) {
  float anim = animation.asSeconds();
  float fillPercent = 1.0f; //0.5f + 0.5f * sin(anim * 5.0f);
  float innerRadius = 15.0f; // 30
  float outerRadius = 20.0f; // 40
  float eyeRadius = innerRadius * 0.7f;
  float eyeIrisDistance = 0.8f;
  float eyeIrisAngle = 2.0f * M_PI * anim;
  float eyeIrisRadius = eyeRadius * 0.6f;
  float eyeBorder = 0.1f;
  float angleOffset = 0.3f * sin(2.0 * M_PI * anim) + vel.x;

  int r = (int)outerRadius;
	for(int x = -r; x < r; x++) {
		for(int y = -r; y < r; y++) {
			if (hypotSqPred(x, y, outerRadius)) {
        sf::Color pixelColor;
        if (hypotSqPred(x, y, eyeRadius)) {
          bool irisTest = hypotSqPred(
            x - eyeIrisDistance * eyeRadius * cos(eyeIrisAngle),
            y - eyeIrisDistance * eyeRadius * sin(eyeIrisAngle),
            eyeIrisRadius
          );
          if (irisTest && hypotSqPred(x, y, eyeRadius * (1.0f - eyeBorder))) {
            pixelColor = sf::Color::Black;
          } else {
            pixelColor = sf::Color::White;
          }
        } else {
          // innerRadius < x < outerRadius
          float angleMultiplier = 4.0f;
          float angle = atan2(y, x) + M_PI;
          float location = (std::hypot(x, y) - innerRadius) / (outerRadius - innerRadius);

          if (location < (sin(2.0f * angle * angleMultiplier + angleOffset) + 1.0f) / 2.0f) {
            if (fillPercent < 0.999f) {
              if (angle > 2.0f * M_PI * fillPercent) {
                if (location > 0.02f) {
                  continue;
                } else {
                  pixelColor = sf::Color::Black;
                  /*copied from below, TODO: restructure */
                }
              }
            } else {
              pixelColor = sf::Color::Black;
            }
          } else {
            continue;
          }
        }
        setPixel(pixels, pos.x + x, pos.y + y, pixelColor);
      }
		}
	}
}
