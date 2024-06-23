#include "points.h"

Points::Points(float _x, float _y, char _type, int _value)
    :Entity(_x, _y, 15, -130, M_PI / 2, 0, 100)
{
    this->typeP = _type;
    this->value = _value;
    if (_type == 's')
        this->texture.loadFromFile("resources\\sprites\\scorePoint.png");
    else
        this->texture.loadFromFile("resources\\sprites\\powerPoint.png");
    this->texture.setSmooth(true);
    this->sprite.setTexture(this->texture);
    this->sprite.setScale(1.6, 1.6);
    this->circHitbox.setRadius(30);
    this->type = 'S';
}
void Points::destroy(char full)
{
        SoundManager::playSound("se_item00.wav");
        if (this->getType() == 'p')
        {
            ScoreManager::changeScores(50);
            ScoreManager::increasePower(1);
        }
        else if (this->getType() == 's')
            ScoreManager::changeScores(this->getValue());
        this->destroyed = true;
}