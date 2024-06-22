#include "bullet.h"

Bullet::Bullet(float _x, float _y, char _owner, float _hitbox_size, float _speed, float _rotation, float _ang_speed_of_rotation, float _acceleration)
    :Entity(_x, _y, _hitbox_size, _speed, _rotation, _ang_speed_of_rotation, _acceleration)
{
    this->owner = _owner;
    if (this->owner == 'E')
        this->type = 'B';
    else
        this->type = 'b';
}