#include <cmath>

#include "Player.hpp"
#include "Util.hpp"

Player::Player(sf::Vector2f position) {
	pos = position;
}

void Player::tick(const sf::Time& dt, std::vector<Entity::ptr>& entities) {

}

void Player::render(sf::Uint8* pixels, sf::FloatRect camera) {
	for(int x = -20; x < 20; x++) {
		for(int y = -20; y < 20; y++) {
			if(std::hypot(x, y) < 20) {
				setPixel(pixels, camera, {pos.x + x, pos.y + y}, sf::Color::Black);
			}
		}
	}
}

