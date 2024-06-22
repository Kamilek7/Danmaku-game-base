#pragma once
#include "entity.h"
#include "danmaku.h"

class Enemy :public Entity
{
    int type_;
    int variant;
    int position;
    bool change;
    float hp;
    float timer;
    bool boss;
public:
    Enemy(bool _boss = false, int _type = 0, int _variant = 0, int _position = 0, float _x = 0, float _y = 0, float _hp = 5, float _hitbox_size = 7, float _speed = 0, float _rotation = 0, float _ang_speed_of_rotation = 0, float _acceleration = 0);
    int getType() { return this->type_; }
    int getVariant() { return this->variant; }
    void process(float dt);
    void destroy( char full = 'n');
    void doWhenReady(float dt);
    char movingDirection() { return 'B'; }
};