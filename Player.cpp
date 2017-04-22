#include <cmath>

#include "Player.hpp"
#include "Util.hpp"

Player::Player(sf::Vector2f position) {
	pos = position;
}

void Player::tick(const sf::Time& dt, std::vector<Entity::ptr>& entities) {
  animation += dt;
}

void Player::render(sf::Uint8* pixels, sf::FloatRect camera) {
  float anim = animation.asSeconds();
  float innerRadius = 30.0f;
  float outerRadius = 40.0f;
  float eyeRadius = innerRadius * 0.7f;
  float eyeIrisDistance = 0.8f;
  float eyeIrisAngle = 2.0f * M_PI * anim;
  float eyeIrisRadius = eyeRadius * 0.6f;
  float eyeBorder = 0.1f;

  int r = (int)outerRadius;
	for(int x = -r; x < r; x++) {
		for(int y = -r; y < r; y++) {
			/*if(std::hypot(x, y) < 20) {
				setPixel(pixels, pos.x + x, pos.y + y, sf::Color::Black);
			}*/
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
          float angleMultiplier = 2.0f;
          float angle = atan2(y, x);
          float location = (std::hypot(x, y) - innerRadius) / (outerRadius - innerRadius);
          // (sin(2x) + 1) / 2
          if (location < (sin(2.0f * angle * angleMultiplier) + 1.0f) / 2.0f) {
             pixelColor = sf::Color::Green;
          } else {
            continue;
          }
        }
        setPixel(pixels, pos.x + x, pos.y + y, pixelColor);
      }
		}
	}
}
