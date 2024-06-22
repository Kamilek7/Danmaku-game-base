#pragma once
#include "bullet.h"
#include "constants.h"
class DanmakuManager
{
    static cords playerPos;
public:
    static std::vector <Bullet*> bullets;
    static void setPlayerPos(cords pos);
	static void createBullet(float _x = 0, float _y = 0, char owner = 'E', float _hitbox_size = 4, float _speed = 0, float _rotation = 0, float _ang_speed_of_rotation = 0, float _acceleration = 0);
    //Spellcards
    static void EternalMeek(int difficulty, cords pos);
    static void PsychadelicRings(int difficulty, cords pos, float dt);
    static void GridSheet(cords pos, float dt, int difficulty);
    //Enemy shots
    static void CircularShot(cords pos, int amount, bool timed = false, float T = 0, float dt = 0);
    static void SnowflakeShot(cords pos, int amount, bool timed = false, float T = 0, float dt = 0);
    static void HomingUShot(cords pos, int amount, bool timed = false, float T = 0, float dt = 0);

};