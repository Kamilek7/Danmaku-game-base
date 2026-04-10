#pragma once
#include "entity.h"

class Points :public Entity
{
    char typeP;
    int value;
public:
    Points(float _x = 0, float _y = 0, char _type = 's', int _value = 0);
    char getType() { return this->typeP; }
    int getValue() { return this->value; }
    void destroy( char full = 'n');
    void process(float dt) { this->Entity::process(dt); }
    void doWhenReady( float dt) { ; }
    char movingDirection() { return 'B'; }
};