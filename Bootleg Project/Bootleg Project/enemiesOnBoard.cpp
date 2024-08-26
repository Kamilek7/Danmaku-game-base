#include "enemiesOnBoard.h"
std::vector<Enemy*> EnemyManager::enemies = {};

void EnemyManager::createEnemy(bool _boss, int _type, int _variant, int _position, float _x, float _y, float _hp, float _hitbox_radius, float _speed, float _rotation, float _ang_speed_of_rotation, float _acceleration)
{
    static int count = 0;
    count++;
    Enemy* temp = new Enemy(_boss, _type, _variant, _position, _x, _y, _hp, _hitbox_radius, _speed, _rotation, _ang_speed_of_rotation, _acceleration);
    enemies.insert(enemies.end(), temp);
}
void EnemyManager::enemiesInWaves(int type, int variant)
{
    if (type == 1)
    {
        createEnemy(false, type, variant, 1, GAME_WIDTH - 32, GAME_HEIGHT / 5 - 12, 9, 12, 70, M_PI, 0, 0);
        createEnemy(false, type, variant, 0, GAME_WIDTH - 32, GAME_HEIGHT / 5 + 24, 9, 12, 70, M_PI, 0, 0);
        createEnemy(false, type, variant, 1, 32, GAME_HEIGHT / 5 - 12, 5, 12, 70, 0, 0, 0);
        createEnemy(false, type, variant, 0, 32, GAME_HEIGHT / 5 + 24, 5, 12, 70, 0, 0, 0);
    }
    else if (type == 2)
    {
        if (variant == 0)
        {
            for (int i = 0; i < 5; i++)
                createEnemy(false, type, variant, 0, GAME_WIDTH - 32, GAME_HEIGHT / 8 + (rand() % 10) * 20, 4, 12, (200 + (rand() % 10) * 20), M_PI - 0.2, 0, 30);
        }
        else
        {
            for (int i = 0; i < 5; i++)
                createEnemy(false, type, variant, 0, 24, GAME_HEIGHT / 8 + (rand() % 10) * 20, 4, 12, 200 + (rand() % 10) * 20, -0.2, 0, 30);
        }
    }
    else if (type == 3)
    {
        if ((int)((variant + 1) / 3) == 0)
        {
            createEnemy(false, type, variant, 0, GAME_WIDTH * 0.35 - 24, H_OFFSET, 10, 16, (100), M_PI / 2, 0, 30);
            createEnemy(false, type, variant, 0, GAME_WIDTH * 0.65 - 24, H_OFFSET, 10, 16, (100), M_PI / 2, 0, 30);
        }
        else
        {
            createEnemy(false, type, variant, 0, GAME_WIDTH - 32, 0, 10, 16, (100), M_PI / 4 * 3, 0, 30);
            createEnemy(false, type, variant, 0, 24, 0, 10, 16, (100), M_PI / 4, 0, 30);
        }
    }
    else if (type == 4)
    {
        createEnemy(false, type, variant, 0, GAME_WIDTH * 0.2 - 24, H_OFFSET, 50, 25, (20), M_PI / 2, -0.3, 10);
        createEnemy(false, type, variant, 0, GAME_WIDTH * 0.8 - 24, H_OFFSET, 50, 25, (20), M_PI / 2, 0.3, 10);
    }
    else if (type == 5)
    {
        createEnemy(false, type, variant, 0, GAME_WIDTH * 0.2 - 24, GAME_HEIGHT - 90, 20, 20, (50), 3 * M_PI / 2, 0.5, 10);
        createEnemy(false, type, variant, 0, GAME_WIDTH * 0.8 - 24, GAME_HEIGHT - 90, 20, 20, (50), 3 * M_PI / 2, -0.5, 10);
    }
}

void EnemyManager::enemyUpdate(float dt)
{
    for (int i = 0; i < enemies.size(); i++)
    {
        if (enemies[i]->out_of_bounds() || enemies[i]->isDestroyed())
        {
            delete enemies[i];
            enemies[i] = nullptr;
            enemies.erase(enemies.begin() + i);
        }
            
        else
        {
            sprites.AnimationTypes[1].idle.playE(enemies[i]->animTimerGet());
            enemies[i]->updateSprite();
            enemies[i]->process(dt);
            if (enemies[i]->isReady())
            {
                enemies[i]->readyChange(false);
                enemies[i]->doWhenReady(dt);
            }
        }


    }
}