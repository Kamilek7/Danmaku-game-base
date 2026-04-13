#include "entity.h"

Entity::Entity(float _x, float _y, float _hitbox_size, float _speed, float _rotation, float _ang_speed_of_rotation, float _acceleration, char _type)
{
    static int num = 0;
    num++;
    if (num > 1000)
        num = 2;
    this->destroyed = false;
    this->hitbox_size = _hitbox_size;
    if (_x == 0)
        this->x = GAME_WIDTH / 2;
    else
        this->x = _x;
    if (_y == 0)
        this->y = GAME_HEIGHT / 4;
    else
        this->y = _y;
    this->speed = _speed;
    this->ang_speed_of_rotation = _ang_speed_of_rotation;
    this->rotation = _rotation;
    this->acceleration = _acceleration;
    this->color = sf::Color::White;
    this->circHitbox.setRadius(hitbox_size);
    this->circHitbox.setOrigin(this->hitbox_size, this->hitbox_size);
    this->circHitbox.setPosition(this->x, this->y);
    this->cooldown = false;
}
void Entity::process(float dt)
{
    this->rotation = this->rotation + dt * this->ang_speed_of_rotation;
    this->speed = this->speed + dt * this->acceleration;
    this->x = this->x + dt * this->speed * cos(this->rotation);
    this->y = this->y + dt * this->speed * sin(this->rotation);
    if (this->checkType() == 'B' || this->checkType() == 'b')
        this->sprite.setPosition(this->x + W_OFFSET, this->y + H_OFFSET);
    else
        this->sprite.setPosition(this->x + W_OFFSET + (this->sprite.getScale().x / 4) * (this->sprite.getTexture())->getSize().x, this->y + H_OFFSET + (this->sprite.getScale().y / 4) * (this->sprite.getTexture())->getSize().y);
    this->circHitbox.setPosition(this->x + W_OFFSET, this->y + H_OFFSET);
    this->sprite.setColor(this->color);
    this->circHitbox.setFillColor(this->color);
    this->animTimer += dt;
}
cords Entity::getLocation()
{
    cords temp;
    temp.x = this->x;
    temp.y = this->y;
    return temp;
}