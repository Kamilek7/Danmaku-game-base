#pragma once
#include "animatedSprite.h"
class Animation
{
	std::string entityName;
	float scale;
public:
	AnimatedSprite idle;
	AnimatedSprite left;
	AnimatedSprite right;
	Animation(std::string _entityName = "reimu", float _scale = 1.6);
	Animation operator=(const Animation& temp) { return Animation(temp.entityName, temp.scale); }
};