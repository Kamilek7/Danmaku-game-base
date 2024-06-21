#ifndef ANIMATION_H_
#define ANIMATION_H_
#include "gameSpecificConstants.h"
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
#endif