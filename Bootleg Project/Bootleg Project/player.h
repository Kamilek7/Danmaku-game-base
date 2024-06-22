#pragma once
#include "entity.h"
#include "danmaku.h"

class Player :public Entity
{
    bool cooldown;
    float xSpeed;
    float ySpeed;
public:
    bool shooting;
    Player();
    bool isDead();
    void moveX(bool A, bool D);
    void moveY(bool W, bool S);
    char movingDirection() { if (this->xSpeed > 0) return 'R'; else if (this->xSpeed < 0) return 'L'; else return 'N'; }
    void process(float dt);
    void destroy(char full = 'n');
    void doWhenReady(float dt);
    void setToStartLocation() { this->x = GAME_WIDTH / 2; this->y = GAME_HEIGHT * 3 / 4; }
};