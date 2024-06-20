#include "gameSpecificConstants.h"
#include <algorithm>

AnimatedSprite AnimatedSprite::operator=(const AnimatedSprite& temp)
{
	if (this == &temp)
		return *this;
	this->baseName = temp.baseName;
	this->baseSprite = temp.baseSprite;
	this->baseTextures = temp.baseTextures;
	this->currentFrame = temp.currentFrame;
	this->frameNum = temp.frameNum;
	this->repeat = temp.repeat;
	return *this;
}

AnimatedSprite::AnimatedSprite(int _frameNum, std::string _baseName, float _scale, bool _repeat)
{
	if (_baseName != "anim")
	{
		this->enTimer = 0;
		this->repeat = _repeat;
		this->frameNum = _frameNum;
		this->baseName = _baseName;
		this->timer = 0;
		this->scale = _scale;
		for (int i = 1; i <= _frameNum; i++)
		{
			sf::Texture temp;
			temp.loadFromFile("resources\\sprites\\" + _baseName + std::to_string(i) + ".png");
			temp.setSmooth(true);
			this->baseTextures.push_back(temp);
			this->currentFrame = 0;
		}
		this->baseSprite.setTexture(this->baseTextures[0]);
		this->baseSprite.setScale(_scale, _scale);
	}
}
void AnimatedSprite::play(float dt)
{
	if (this->frameNum > 1)
	{
		this->timer += dt;
		if (this->timer > 1 / float(framerate))
		{
			this->timer = 0;
			this->currentFrame++;
			if (this->currentFrame >= this->frameNum)
			{
				if (this->repeat)
					this->currentFrame = 0;
				else
					this->currentFrame = this->frameNum - 1;
			}
			this->baseSprite.setTexture(this->baseTextures[this->currentFrame]);
		}
	}
}
void AnimatedSprite::playE(float enemTime)
{
	if (this->frameNum > 1)
	{
		int current = enemTime / (1 / float(framerate));
		if (this->repeat)
			current = current % frameNum;
		if (!this->repeat && current >= frameNum)
			current = frameNum-1;
		this->baseSprite.setTexture(this->baseTextures[current]);
	}
}
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
