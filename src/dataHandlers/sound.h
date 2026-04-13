#ifndef SOUND_MANAGER_H_
#define SOUND_MANAGER_H_

#include "constants.h"

class SoundManager
{
	static std::map <std::string, sf::SoundBuffer> soundCache;
	static sf::Sound voices[20];
	static sf::SoundBuffer buffers[20];
public:
	static void playSound(std::string filename);
};

#endif