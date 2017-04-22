#pragma once

#include "Entity.hpp"

class Player : public Entity {
public:
	enum State {
		FREE,
		DRAWING,
		FOLLOWING
	};

	const float drawLineDistance = 20.0f;
	const int drawNumClouds = 20;
	const float followVelocity = 400.0f;

	float drawAngle;
	sf::Vector2f drawLast;
	sf::Vector2f drawCurr;

	std::vector<sf::Vector2f> followPoints;
	float followMovedDistance;
	int followCloudProgress;
	float followLaunchAngle;

	State state = FREE;
	float eyeIrisAngle;

	Player(sf::Vector2f position);

	void tick(const sf::Time& dt, std::vector<Entity::ptr>& entities);
	void render(sf::Uint8* pixels, sf::FloatRect& camera);
	void drawCloud(sf::Uint8* pixels, sf::Vector2f center);
};
