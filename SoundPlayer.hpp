#pragma once

#include <SFML/Audio.hpp>
#include <map>
#include <list>

namespace Sound {
	enum ID {
		BLIP,
		BUBBLE,
		BUBOUT,
		EXPLO,
		SUCC,
		DED,
		GOGO,
		HURT,
		RDY
	};
}

class SoundPlayer {
	public:
		SoundPlayer();
		void tick(sf::Time dt);

		sf::Sound& play(Sound::ID id);

	private:
		std::map<Sound::ID, sf::SoundBuffer> buffers;
		std::list<sf::Sound> sounds;
};

extern SoundPlayer soundPlayer;


