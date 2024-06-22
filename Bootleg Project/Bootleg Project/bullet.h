#pragma once
#include "entity.h"
class Bullet :public Entity
{
    char owner;
public:
    Bullet(float _x = 0, float _y = 0, char _owner = 'E', float _hitbox_size = 4, float _speed = 0, float _rotation = 0, float _ang_speed_of_rotation = 0, float _acceleration = 0);
    void destroy(char full = 'n') { this->destroyed = true; }
    void process(float dt) { this->Entity::process(dt); }
    void doWhenReady(float dt) { ; }
    char movingDirection() { return 'B'; }
};