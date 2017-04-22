#include <cmath>

#include "Player.hpp"
#include "Util.hpp"
#include "Consts.hpp"

Player::Player(sf::Vector2f position) {
	pos = position;
    vel.x = -20.0f;
    drawAngle = 0.0f;
    eyeIrisAngle = 0.0f;
}

void Player::tick(const sf::Time& dt, std::vector<Entity::ptr>& entities) {
  float fdt = dt.asSeconds();
  animation += dt;

  switch(state) {
    case FREE:
    {
      vel.x += 8.0f * sf::Joystick::getAxisPosition(0, sf::Joystick::X) * fdt;
      vel.y += 1000.0f * fdt + 7.0f * sf::Joystick::getAxisPosition(0, sf::Joystick::Y) * fdt; // 1000dt cool gravity
      pos.x += vel.x * fdt;
      pos.y += vel.y * fdt;
      if (sf::Joystick::isButtonPressed(0, 0)) {
        state = DRAWING;
        // init drawing
        if (std::hypot(vel.x, vel.y) < 1.0f) {
          drawAngle = atan2(-1.0f, 0.0f);
        } else {
          drawAngle = atan2(vel.y, vel.x);
          drawLast = pos;
          drawCurr = drawLast;
        }
      }
      eyeIrisAngle = atan2(vel.y, vel.x);
      break;
    }
    case DRAWING:
    {
      float joyX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
      float joyY = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

      if (!hypotSqPred(joyX, joyY, 25.0f)) {
        drawAngle = rotateTo(drawAngle, atan2(joyY, joyX), 3.0f * M_PI * fdt);
      }

      if (!hypotSqPred(drawCurr.x - drawLast.x, drawCurr.y - drawLast.y, drawLineDistance)) {
        drawLast = drawCurr;
        followPoints.emplace_back(drawCurr);
        if (followPoints.size() == drawNumClouds) {
          state = FOLLOWING;
          // init following
          followMovedDistance = 0.0f;
          auto it = followPoints.rbegin();
          auto lastPoint = *it;
          auto secondLastPoint = *(std::next(it));
          followLaunchAngle = atan2(lastPoint.y - secondLastPoint.y, lastPoint.x - secondLastPoint.x);
          followCloudProgress = 0;
        }
      }

      drawCurr.x += 200.0f * cos(drawAngle) * fdt;
      drawCurr.y += 200.0f * sin(drawAngle) * fdt;

      eyeIrisAngle = atan2(drawCurr.y - pos.y, drawCurr.x - pos.x);

      break;
    }
    case FOLLOWING:
    {
      // this code assumes that the framerate is constant (60 fps)
      followMovedDistance += followVelocity * fdt;
      auto prev = *(followPoints.begin());
      float accumDistance = 0.0f;
      bool updated = false;
      for (auto& point : followPoints) {
        float length = std::hypot(point.x - prev.x, point.y - prev.y);
        if (accumDistance <= followMovedDistance && followMovedDistance <= accumDistance + length) {
          float t = (accumDistance - followMovedDistance) / length;
          pos.x = point.x + (prev.x - point.x) * t;
          pos.y = point.y + (prev.y - point.y) * t;
          updated = true;
          break;
        }
        prev = point;
        accumDistance += length;
      }
      if (!updated) {
        // set player velocity to launch velocity
        state = FREE;
      }
      break;
    }
  }

  if (pos.y > screenHeight - 20.0f && vel.y > 0.0f) {
    vel.y = -fabs(vel.y) * 0.8f;
    pos.y = screenHeight - 20.0f;
  }
}

void Player::render(sf::Uint8* pixels, sf::FloatRect& camera) {
  float anim = animation.asSeconds();
  float fillPercent = 1.0f; // 0.5f + 0.5f * sin(anim * 5.0f);
  float innerRadius = 15.0f; // 30
  float outerRadius = 20.0f; // 40
  float eyeRadius = innerRadius * 0.7f;
  float eyeIrisDistance = 0.8f;
  //float eyeIrisAngle = atan2(vel.y, vel.x); // 2.0f * M_PI * anim;
  float eyeIrisRadius = eyeRadius * 0.6f;
  float eyeBorder = 0.1f;
  float angleOffset = 0.3f * sin(2.0 * M_PI * anim) - fmod(0.2f * pos.x - screenWidth * 0.5f, 2.0 * M_PI);

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
                }
              }
            } else {
              pixelColor = sf::Color::Black;
            }
          } else {
            continue;
          }
        }
        setPixel(pixels, camera, {pos.x + x, pos.y + y}, pixelColor);
      }
		}
	}

  // draw follow clouds
  if (state == DRAWING || state == FOLLOWING) {
    for (auto& followPoint : followPoints) {
      drawCloud(pixels, followPoint);
    }

    if (state == DRAWING) drawCloud(pixels, drawCurr);
  }
}

void Player::drawCloud(sf::Uint8* pixels, sf::Vector2f center) {
  float anim = animation.asSeconds() + (center.x + center.y);
  float offsetX = 2.0f * (0.2f * cos(12.0f * anim));
  float offsetY = 2.0f * (0.1f * sin(18.0f * anim));
  float radius = 5.0f + (0.3f * sin(3.0f * M_PI * anim));

  int r = 2 + (int)radius;
  for(int x = -r; x < r; x++) {
		for(int y = -r; y < r; y++) {
      // these will be more than just circles later OKSUKA
      if (hypotSqPred(x + offsetX, y + offsetY, radius)) {
        setPixel(pixels, center.x + x, center.y + y, sf::Color::Black);
      }
    }
  }
}
