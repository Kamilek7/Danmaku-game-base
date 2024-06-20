#ifndef GAMEBOARD_H_
#define GAMEBOARD_H_
#define _USE_MATH_DEFINES
#include "gameSpecificConstants.h"
#include <string>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>
const float APP_WIDTH = 1200;
const float APP_HEIGHT = 900;
const float GAME_WIDTH = 720;
const float GAME_HEIGHT = 840;
const float H_OFFSET = 30;
const float W_OFFSET = 60;
const float SPEED = 300;
const int ENEMY_TYPES = 5;
const int MIDBOSS_TYPES = 1;
const int SPELLCARDS = 3;
struct cords
{
    float x = 0;
    float y = 0;
};
std::vector<cords> adjacentTiles(int i, int j);
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
    virtual void destroy(Board* b, char full='n') = 0;
    virtual void doWhenReady(Board* b, float dt) = 0;
    virtual char movingDirection() = 0;
};
class Points:public Entity
{
    char typeP;
    int value;
public:
    Points(float _x = 0, float _y = 0, char _type = 's', int _value = 0);
    char getType() { return this->typeP; }
    int getValue() { return this->value; }
    void destroy(Board* b, char full = 'n');
    void process(float dt) { this->Entity::process(dt); }
    void doWhenReady(Board* b, float dt) { ; }
    char movingDirection() { return 'B'; }
};
class Bullet:public Entity
{
    char owner;
public:
    Bullet(float _x = 0, float _y = 0, char _owner='E', float _hitbox_size = 4, float _speed = 0, float _rotation = 0, float _ang_speed_of_rotation = 0, float _acceleration = 0);
    void destroy(Board* b, char full = 'n') { this->destroyed = true;  }
    void process(float dt) { this->Entity::process(dt); }
    void doWhenReady(Board* b, float dt) { ; }
    char movingDirection() { return 'B'; }
};
class Player:public Entity
{
    int lives;
    bool cooldown;
    float xSpeed;
    float ySpeed;
public:
    bool shooting;
    int power;
    Player();
    bool isDead();
    void moveX(bool A, bool D);
    void moveY(bool W, bool S);
    void gainLife(){ this->lives++; }
    int numberOfLives(){return this->lives;}
    char movingDirection() { if (this->xSpeed > 0) return 'R'; else if (this->xSpeed < 0) return 'L'; else return 'N'; }
    void process(float dt);
    void destroy(Board* b, char full = 'n');
    void doWhenReady(Board* b, float dt);
    void setToStartLocation() { this->x = GAME_WIDTH / 2; this->y = GAME_HEIGHT * 3 / 4; }
};
class Enemy:public Entity
{
    int type_;
    int variant;
    int position;
    bool change;
    float hp;
    float timer;
    bool boss;
public:
    Enemy(bool _boss = false,int _type = 0, int _variant = 0, int _position = 0, float _x = 0, float _y = 0, float _hp = 5, float _hitbox_size = 7, float _speed = 0, float _rotation = 0, float _ang_speed_of_rotation = 0, float _acceleration = 0);
    int getType() { return this->type_; }
    int getVariant() { return this->variant; }
    void process(float dt);
    void destroy(Board *b, char full = 'n');
    void doWhenReady(Board* b, float dt);
    char movingDirection() { return 'B'; }
};
class Board
{
    bool pause;
    long unsigned int score;
    float waveTimer;
    float waitingTimer;
    int internalScore;
    float stage_progress;
    float switch1;
    bool endStage;
    std::vector <Animation> AnimationTypes;
    sf::Texture BulletTextures;
    sf::Sprite BulletSprites;
    sf::Texture BulletTextures1;
    sf::Sprite BulletSprites1;
    sf::Sound voices[10];
    sf::SoundBuffer buffers[10];
public:
    friend class Entity;
    Player *player;
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
    void createBullet(float _x = 0, float _y = 0, char owner='E', float _hitbox_size = 4, float _speed = 0, float _rotation = 0, float _ang_speed_of_rotation = 0, float _acceleration = 0);
    //Spellcards
    void EternalMeek(int difficulty, cords pos);
    void PsychadelicRings(int difficulty, cords pos, float dt);
    void GridSheet(cords pos, float dt, int difficulty);
    //Enemy shots
    void CircularShot(cords pos, int amount, bool timed = false, float T = 0, float dt = 0);
    void SnowflakeShot(cords pos, int amount, bool timed = false, float T = 0, float dt = 0);
    void HomingUShot(cords pos, int amount, bool timed = false, float T = 0, float dt = 0);
    //Metodowe metody
    bool checkCollisionTypes(Entity * a1, Entity * b1);
    void manageCollisions();
    void changeScores(int value);
    void createRandomScorePoints(cords pos);
    void updateEntities(float dt);
    void playSound(std::string filename);
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
#endif