#pragma once
#include "constants.h"
class SoundManager
{
	static sf::Sound voices[10];
	static sf::SoundBuffer buffers[10];
public:
	static void playSound(std::string filename);
};