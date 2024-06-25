#pragma once
#include "enemy.h"

class EnemyManager
{
public:
	static std::vector <Enemy*> enemies;
	static void createEnemy(bool _boss = false, int _type = 0, int _variant = 0, int _position = 0, float _x = 0, float _y = 0, float _hp = 5, float _hitbox_radius = 7, float _speed = 0, float _rotation = 0, float _ang_speed_of_rotation = 0, float _acceleration = 0);
	static void enemiesInWaves(int type = 1, int variant = 0);
	static void enemyUpdate(float dt);
};