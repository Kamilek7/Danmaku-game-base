#pragma once

#include "player.h"
#include "pointsOnBoard.h"
#include "spriteManager.h"
#include "enemiesOnBoard.h"
#include "hud.h"

extern SpriteManager sprites;
extern HUDManager HUDTextures;

class Board
{

    bool pause;
    float waveTimer;
    float waitingTimer;
    float stage_progress;
    float switch1;
    bool endStage;
public:
    Player* player;
    Board();
    //Metodowe metody
    bool checkCollisionTypes(Entity* a1, Entity* b1);
    void manageCollisions();
    void updateEntities(float dt);
    void waveProcess(float dt);
    void gameProcess(float dt, int gameMode);
    void restart();
    void pauseUnpause() { this->pause = !this->pause; }
    bool checkPause() { return this->pause; }
    bool isEndOfStage() { return this->endStage; }
    void nextStage() { this->endStage = false; this->stage_progress = 0; this->player->setToStartLocation(); }
};