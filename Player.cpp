#include <cmath>
#include <iostream>

#include "ParticleSystem.hpp"
#include "Player.hpp"
#include "Util.hpp"
#include "Consts.hpp"
#include "Bubble.hpp"

Player::Player(sf::Vector2f position) {
	pos = position;
	drawAngle = 0.0f;
	eyeIrisAngle = 0.0f;
	cooldown = 0.0f;
	id = Entity::PLAYER;

	bubbleConfs[0] = {250.f, 6};
	bubbleConfs[1] = {600.f, 7};
}

void Player::tick(const sf::Time& dt, std::vector<Entity::ptr>& entities) {
	sf::Time rdt = (state != FREE ? dt * drawSlowdown : dt);
	float fdt = rdt.asSeconds();
	animation += rdt;

	if (state == FREE && cooldown > 0.0f) {
		cooldown -= fdt;
	}

	switch(state) {
		case FREE:
			{
				vel.x += 8.0f * sf::Joystick::getAxisPosition(0, sf::Joystick::X) * fdt;
				vel.y += 700.0f * fdt; // + 7.0f * sf::Joystick::getAxisPosition(0, sf::Joystick::Y) * fdt; // 1000dt cool gravity
				pos.x += vel.x * fdt;
				pos.y += vel.y * fdt;
				if (cooldown <= 0.0f && sf::Joystick::isButtonPressed(0, 0)) {
					state = DRAWING;
					// init drawing
					madeBubble = false;
					followLastPassedIndex = 0;
					followPoints.clear();
					followPoints.emplace_back(pos);
					if (std::hypot(vel.x, vel.y) < 1.0f) {
						drawAngle = atan2(-1.0f, 0.0f);
					} else {
						drawAngle = atan2(vel.y, vel.x);
						drawLast = pos;
						drawCurr = drawLast;
					}
				}
				eyeIrisAngle = rotateTo(eyeIrisAngle, atan2(vel.y, vel.x), 2.0 * M_PI * fdt);
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

					// Check bubble
					for(auto& conf : bubbleConfs) {
						auto bubblePoints = conf.second;
						auto distEps = conf.first;

						if(followPoints.size() >= bubblePoints && !madeBubble) {
							sf::Vector2f center(0, 0);
							for(auto it = followPoints.rbegin(); it != followPoints.rbegin() + bubblePoints; it++) {
								center += *it / (float)bubblePoints;
							}

							float minDist = 100000.f;
							float maxDist = 0.f;
							for(auto it = followPoints.rbegin(); it != followPoints.rbegin() + bubblePoints; it++) {
								sf::Vector2f p = center - *it;
								float d = hypotSq(p.x, p.y);
								minDist = std::min(minDist, d);
								maxDist = std::max(maxDist, d);
							}

							if(maxDist - minDist <= distEps) {
								float r = std::sqrt((maxDist + minDist) / 2.f);
								entities.emplace_back(new Bubble(center, r));
								madeBubble = true;
							}
						}
					}

					// Finish drawing
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

				cooldown = cooldownTime * ((float)followPoints.size() / (float)drawNumClouds);

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
				int i = 0;
				for (auto& point : followPoints) {
					//if(point == prev) continue;
					float length = std::hypot(point.x - prev.x, point.y - prev.y);
					if (accumDistance <= followMovedDistance && followMovedDistance <= accumDistance + length) {
						float t = (accumDistance - followMovedDistance) / length;
						pos.x = prev.x + (prev.x - point.x) * t;
						pos.y = prev.y + (prev.y - point.y) * t;
						updated = true;
						eyeIrisAngle = rotateTo(eyeIrisAngle, atan2(point.y - prev.y, point.x - prev.x), 2.0 * M_PI * fdt);
						followLastPassedIndex = i - 1;
						break;
					}
					accumDistance += length;
					prev = point;
					i++;
				}
				if (!updated) {
					// set player velocity to launch velocity
					vel.x = followVelocity * cos(followLaunchAngle);
					vel.y = followVelocity * sin(followLaunchAngle);
					state = FREE;
					cooldown = cooldownTime;
				}
				break;
			}
	}

	if (pos.y > screenHeight - outerRadius && vel.y > 0.0f) {
		vel.y = 0.0f;
		pos.y = screenHeight - outerRadius;
	}

	if(pos.x - outerRadius < 0) vel.x = 0, pos.x = outerRadius;
	else if(pos.x + outerRadius >= screenWidth) vel.x = 0, pos.x = screenWidth - outerRadius;
}

void Player::render(sf::Uint8* pixels, sf::FloatRect& camera) {
	float anim = animation.asSeconds();
	float fillPercent = (cooldown < 0.001f ? 1.0f : (1.0f - (cooldown / cooldownTime))); // 0.5f + 0.5f * sin(anim * 5.0f);
	float eyeRadius = innerRadius * 0.7f;
	float eyeIrisDistance = 0.8f;
	//float eyeIrisAngle = atan2(vel.y, vel.x); // 2.0f * M_PI * anim;
	float eyeIrisRadius = eyeRadius * 0.6f;
	float eyeBorder = 0.1f;
	float angleOffset = 0.3f * sin(2.0 * M_PI * anim) - fmod(0.2f * pos.x - screenWidth * 0.5f, 2.0 * M_PI);

	// draw follow clouds
	if (state == DRAWING || state == FOLLOWING) {
		int i = 0;
		for (auto& followPoint : followPoints) {
			if (i > followLastPassedIndex) {
				drawCloud(pixels, camera, followPoint);
			}
			i++;
		}

		if (state == DRAWING) drawCloud(pixels, camera, drawCurr);
	}

	// draw GEMUSAN
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
}

void Player::drawCloud(sf::Uint8* pixels, sf::FloatRect camera, sf::Vector2f center) {
	float anim = animation.asSeconds() + (center.x + center.y);
	float offsetX = 2.0f * (0.2f * cos(12.0f * anim));
	float offsetY = 2.0f * (0.1f * sin(18.0f * anim));
	float radius = 5.0f + (0.3f * sin(3.0f * M_PI * anim));

	int r = 2 + (int)radius;
	for(int x = -r; x < r; x++) {
		for(int y = -r; y < r; y++) {
			// these will be more than just circles later OKSUKA
			if (hypotSqPred(x + offsetX, y + offsetY, radius)) {
				setPixel(pixels, camera, {center.x + x, center.y + y}, sf::Color::Black);
			}
		}
	}
}

void Player::kill() {
	if(!dead) {
		ParticleSystem::getInstance()->explode(pos);
		pos = {50.f, 50.f};
		//dead = true;
	}
}
