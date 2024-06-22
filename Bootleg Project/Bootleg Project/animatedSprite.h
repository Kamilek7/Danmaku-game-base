#pragma once

#include "constants.h"

class AnimatedSprite
{
	const int framerate = 12;
	bool repeat;
	int frameNum;
	int currentFrame;
	float timer;
	float scale;
	float enTimer;
	std::string baseName;
	std::vector <sf::Texture> baseTextures;
public:
	sf::Sprite baseSprite;
	AnimatedSprite operator=(const AnimatedSprite& temp);
	AnimatedSprite(int _frameNum = 1, std::string _baseName = "anim", float _scale = 1.0, bool _repeat = true);
	void play(float dt);
	void playE(float enemTime);
	void reset() { this->currentFrame = 0; }
};
