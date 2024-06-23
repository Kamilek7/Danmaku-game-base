#include "spriteManager.h"

SpriteManager::SpriteManager()
{
    BulletTextures.loadFromFile("resources\\sprites\\bullet1a.png");
    BulletTextures.setSmooth(true);
    BulletSprites.setTexture(BulletTextures);
    BulletTextures1.loadFromFile("resources\\sprites\\playerBullet1.png");
    BulletTextures1.setSmooth(true);
    BulletSprites1.setTexture(BulletTextures1);
    Animation reimu("reimu", 1.6);
    AnimationTypes.push_back(reimu);
    Animation enemy("enemy", 1.6);
    AnimationTypes.push_back(enemy);
}

SpriteManager sprites = SpriteManager();