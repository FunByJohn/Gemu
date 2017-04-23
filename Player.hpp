#pragma once

#include "Entity.hpp"
#include <utility>
#include <array>

class Player : public Entity {
private:
	bool madeBubble = false;

	std::array<std::pair<float, int>, 2> bubbleConfs;

public:
	enum State {
		FREE,
		DRAWING,
		FOLLOWING
	};

	const float drawLineDistance = 20.0f;
	const int drawNumClouds = 12;
	const float followVelocity = 400.0f; //400.0f;
	const float cooldownTime = 1.0f;

	float innerRadius = 15.f;
	float outerRadius = 20.f;

	float drawAngle;
	sf::Vector2f drawLast;
	sf::Vector2f drawCurr;
	sf::Time deadTime = sf::Time::Zero;

  std::vector<sf::Vector2f> followPoints;
  float followMovedDistance;
  int followCloudProgress;
  float followLaunchAngle;
  int followLastPassedIndex;

	State state = FREE;
	float eyeIrisAngle;

  float cooldown;

	Player(sf::Vector2f position);

	void tick(const sf::Time& dt, std::vector<Entity::ptr>& entities);
	void render(sf::Uint8* pixels, sf::FloatRect& camera);
  	void drawCloud(sf::Uint8* pixels, sf::FloatRect camera, sf::Vector2f center);

	void kill();
	void reset();
};
