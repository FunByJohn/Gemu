#include "Player.hpp"

class GoodThing : public Entity {
private:
	Player* player;

public:
	GoodThing(sf::Vector2f pos, Player* player);

	void tick(const sf::Time& dt, Entity::container& entities);
	void render(sf::Uint8* pixels, sf::FloatRect& camera);
};
