#include "animation.h"
Animation::Animation(std::string _entityName, float _scale)
{
	this->entityName = _entityName;
	this->scale = _scale;
	if (this->entityName == "reimu")
	{
		this->idle.FillInfo(8, _entityName + "Idle", _scale, true);
		this->left.FillInfo(8, _entityName + "Left", _scale, false);
		this->right.FillInfo(8, _entityName + "Right", _scale, false);
	}
	else
	{
		this->idle.FillInfo(4, _entityName + "Idle", _scale, true);
		this->left.FillInfo(4, _entityName + "Left", _scale, false);
		this->right.FillInfo(4, _entityName + "Right", _scale, false);
	}
}