#pragma once

#include "constants.h"
#include "animation.h"

class SpriteManager
{
public:
    sf::Texture BulletTextures;
    sf::Sprite BulletSprites;
    sf::Texture BulletTextures1;
    sf::Sprite BulletSprites1;
    std::vector <Animation> AnimationTypes;
	SpriteManager();
};
