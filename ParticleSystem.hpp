#include <vector>

#include "Entity.hpp"

class Particle : public Entity {
private:
	sf::Vector2f acc;

public:
	enum Type {
		PIXEL
	};

	Type type;
	sf::Time liveTime;
	sf::Time elapsed;
	sf::Color color;

	Particle(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f acc, sf::Time liveTime, Type type, sf::Color color);
	Particle(std::vector<sf::Vector2f> opt, sf::Time liveTime, Type type, sf::Color color);

	void tick(const sf::Time& dt, Entity::container& entities);
	void render(sf::Uint8* pixels, sf::FloatRect camera);
};

