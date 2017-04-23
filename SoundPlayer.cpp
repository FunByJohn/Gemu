#include "SoundPlayer.hpp"

// Load sounds
SoundPlayer::SoundPlayer() {
	buffers[Sound::BLIP].loadFromFile("res/blip.wav");
	buffers[Sound::BUBBLE].loadFromFile("res/bubble.wav");
	buffers[Sound::BUBOUT].loadFromFile("res/bubout.wav");
	buffers[Sound::EXPLO].loadFromFile("res/explo.wav");
	buffers[Sound::SUCC].loadFromFile("res/succ.wav");
	buffers[Sound::DED].loadFromFile("res/ded.wav");
	buffers[Sound::GOGO].loadFromFile("res/gogo.wav");
}


void SoundPlayer::tick(sf::Time dt) {
	for(auto it = sounds.begin(); it != sounds.end();) {
		if(it->getStatus() == sf::Sound::Stopped)
			it = sounds.erase(it);
		else
			it++;
	}
}

#include <iostream>
sf::Sound& SoundPlayer::play(Sound::ID id) {
	if(buffers.find(id) == buffers.end())
		std::cout << "Couldn't find sound!" << std::endl;

	sounds.emplace_back(buffers[id]);
	sf::Sound& sound = sounds.back();
	sound.play();

	return sound;
}

SoundPlayer soundPlayer;

