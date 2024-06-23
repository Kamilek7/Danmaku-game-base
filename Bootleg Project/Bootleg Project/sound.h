#pragma once
#include "constants.h"
class SoundManager
{
	static sf::Sound voices[20];
	static sf::SoundBuffer buffers[20];
public:
	static void playSound(std::string filename);
};