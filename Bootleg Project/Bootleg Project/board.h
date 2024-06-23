#pragma once

#include "entity.h"
#include "animation.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "points.h"
#include "pointsOnBoard.h"
#include "spriteManager.h"


extern SpriteManager sprites;

class Board
{
    bool pause;
    float waveTimer;
    float waitingTimer;
    float stage_progress;
    float switch1;
    bool endStage;
public:
    friend class Entity;
    Player* player;
    sf::Texture GameBackground;
    sf::Texture HUDt;
    sf::Texture MainMenuT;
    sf::Sprite HUD;
    sf::Sprite GAMEBOARD;
    sf::Sprite MainMenu;
    sf::Text Score;
    sf::Text Power;
    sf::Text Lives;
    std::vector <sf::Text> Options;
    std::vector <Entity*> entities;
    Board();
    //Metodowe metody
    bool checkCollisionTypes(Entity* a1, Entity* b1);
    void manageCollisions();
    void updateEntities(float dt);
    void createEnemy(bool _boss = false, int _type = 0, int _variant = 0, int _position = 0, float _x = 0, float _y = 0, float _hp = 5, float _hitbox_radius = 7, float _speed = 0, float _rotation = 0, float _ang_speed_of_rotation = 0, float _acceleration = 0);
    void enemiesInWaves(int type = 1, int variant = 0);
    void waveProcess(float dt);
    void gameProcess(float dt, int gameMode);
    void restart();
    void pauseUnpause() { this->pause = !this->pause; }
    bool checkPause() { return this->pause; }
    bool isEndOfStage() { return this->endStage; }
    void nextStage() { this->endStage = false; this->stage_progress = 0; this->player->setToStartLocation(); }
    void a() { this->endStage = true; }
};