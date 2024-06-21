#include "animation.h"
Animation::Animation(std::string _entityName, float _scale)
{
	this->entityName = _entityName;
	this->scale = _scale;
	if (this->entityName == "reimu")
	{
		this->idle = AnimatedSprite(8, _entityName + "Idle", _scale, true);
		this->left = AnimatedSprite(8, _entityName + "Left", _scale, false);
		this->right = AnimatedSprite(8, _entityName + "Right", _scale, false);
	}
	else
	{
		this->idle = AnimatedSprite(4, _entityName + "Idle", _scale, true);
		this->left = AnimatedSprite(4, _entityName + "Left", _scale, false);
		this->right = AnimatedSprite(4, _entityName + "Right", _scale, false);
	}
}