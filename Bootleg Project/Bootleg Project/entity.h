#pragma once
#include "constants.h"

class Entity
{
protected:
    float x;
    float y;
    float hitbox_size;
    float rotation;
    float ang_speed_of_rotation;
    float speed;
    float acceleration;
    char type;
    bool destroyed;
    bool ready;
    float animTimer;
    sf::CircleShape circHitbox;
    sf::Color color;
    sf::Texture texture;
    sf::Sprite sprite;
public:
    friend class Board;
    Entity(float _x = 0, float _y = 0, float _hitbox_size = 4, float _speed = 0, float _rotation = 0, float _ang_speed_of_rotation = 0, float _acceleration = 0, char _type = 'B');
    bool out_of_bounds() { if (this->x > GAME_WIDTH || this->x < 0 || this->y > GAME_HEIGHT || this->y < 0) return true; else return false; }
    cords getLocation();
    float getSize() { return this->hitbox_size; }
    char checkType() { return this->type; }
    sf::Sprite getSprite() { return this->sprite; }
    sf::CircleShape getHitboxC() { return this->circHitbox; }
    bool isDestroyed() { return this->destroyed; }
    bool isReady() { return this->ready; }
    void readyChange(bool f) { this->ready = f; }
    virtual void process(float dt);
    virtual void destroy(char full = 'n') = 0;
    virtual void doWhenReady(float dt) = 0;
    virtual char movingDirection() = 0;
};