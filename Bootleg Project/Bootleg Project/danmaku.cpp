#include "danmaku.h"

cords DanmakuManager::playerPos = cords { 0, 0 };
std::vector <Bullet*> DanmakuManager::bullets = {};

void DanmakuManager::setPlayerPos(cords pos)
{
    playerPos = pos;
}

void DanmakuManager::createBullet(float _x, float _y, char owner, float _hitbox_size, float _speed, float _rotation, float _ang_speed_of_rotation, float _acceleration)
{
    Bullet* temp = new Bullet(_x, _y, owner, _hitbox_size, _speed, _rotation, _ang_speed_of_rotation, _acceleration);
    bullets.insert(bullets.end(), temp);
}
void DanmakuManager::EternalMeek(int difficulty, cords pos)
{
    if (!(rand() % difficulty))
    {
        SoundManager::playSound("se_tan00.wav");
        createBullet(pos.x, pos.y, 'E', 4, (250 + round(250 * rand() % 2)), (float)rand() / (float)RAND_MAX * (2 * M_PI), 0, 10);
    }
}
void DanmakuManager::PsychadelicRings(int difficulty, cords pos, float dt)
{
    static float time_elapsed = 0;
    static bool ver = false;
    time_elapsed += dt;
    float dif = float(rand() % 3) / 15.f;
    if (time_elapsed > 0.5)
    {
        int amount = (difficulty - 8) / 4;
        SoundManager::playSound("se_tan00.wav");
        time_elapsed = 0;
        ver = !ver;
        for (int i = 0; i < difficulty; i++)
            createBullet(pos.x + 2, pos.y + 2, 'E', 4, 10, 2 * M_PI / difficulty * i, (1 - (2 * ver)) * (0.5 + dif), 50);
        if (amount > 0)
        {
            float degToPlayer = acos((playerPos.x - pos.x) / sqrt((playerPos.x - pos.x) * (playerPos.x - pos.x) + (playerPos.y - pos.y) * (playerPos.y - pos.y)));
            if (playerPos.y - pos.y < 0)
                degToPlayer += M_PI;
            for (int i = -int(amount / 2); i <= int(amount / 2); i++)
                createBullet(pos.x, pos.y, 'E', 4, 10, degToPlayer + i * 0.1, 0, 80);
        }
    }
}
void DanmakuManager::GridSheet(cords pos, float dt, int difficulty)
{
    static float time_elapsed = 0;
    time_elapsed += dt;
    float newXL = pos.x - time_elapsed * 300;
    float newXR = pos.x + time_elapsed * 300;
    if (!(int(time_elapsed * 200) % 70) && newXL > 0)
    {
        SoundManager::playSound("se_tan00.wav");
        for (int i = 0; i < difficulty; i++)
            createBullet(newXR, pos.y, 'E', 4, 10, 2 * M_PI / difficulty * i, 0, 50);
        for (int i = 0; i < difficulty; i++)
            createBullet(newXL, pos.y, 'E', 4, 10, 2 * M_PI / difficulty * i, 0, 50);
    }
    if (time_elapsed > 3)
        time_elapsed = 0;
}
void DanmakuManager::CircularShot(cords pos, int amount, bool timed, float T, float dt)
{
    if (timed)
    {
        static float time_elapsed = 0;
        time_elapsed += dt;
        if (time_elapsed > T)
        {
            SoundManager::playSound("se_tan00.wav");
            time_elapsed = 0;
            for (int i = 0; i < amount; i++)
                createBullet(pos.x + 2, pos.y + 2, 'E', 4, 10, 2 * M_PI / amount * i, 0, 50);
        }
    }
    else
    {
        SoundManager::playSound("se_tan00.wav");
        for (int i = 0; i < amount; i++)
            createBullet(pos.x + 2, pos.y + 2, 'E', 4, 10, 2 * M_PI / amount * i, 0, 50);
    }
}
void DanmakuManager::SnowflakeShot(cords pos, int amount, bool timed, float T, float dt)
{
    if (timed)
    {
        static float time_elapsed = 0;
        time_elapsed += dt;
        if (time_elapsed > T)
        {
            SoundManager::playSound("se_tan00.wav");
            time_elapsed = 0;
            for (int j = 1; j < 5; j++)
            {
                for (int i = 0; i < amount; i++)
                    createBullet(pos.x + 2, pos.y + 2, 'E', 4, 10, 2 * M_PI / amount * i, 0, j * 30);
            }
        }
    }
    else
    {
        SoundManager::playSound("se_tan00.wav");
        for (int j = 1; j < 5; j++)
        {
            for (int i = 0; i < amount; i++)
                createBullet(pos.x + 2, pos.y + 2, 'E', 4, 10, 2 * M_PI / amount * i, 0, j * 30);
        }
    }
}
void DanmakuManager::HomingUShot(cords pos, int amount, bool timed, float T, float dt)
{
    if (timed)
    {
        static float time_elapsed = 0;
        time_elapsed += dt;
        if (time_elapsed > T)
        {
            SoundManager::playSound("se_tan00.wav");
            time_elapsed = 0;
            float degToPlayer = acos((playerPos.x - pos.x) / sqrt((playerPos.x - pos.x) * (playerPos.x - pos.x) + (playerPos.y - pos.y) * (playerPos.y - pos.y)));
            if (playerPos.y - pos.y < 0)
                degToPlayer += M_PI;

            for (int i = -int(amount / 2); i <= int(amount / 2); i++)
                createBullet(pos.x, pos.y, 'E', 4, 50, degToPlayer + i * 0.1, 0, 50);
        }
    }
    else
    {
        float degToPlayer = acos((playerPos.x - pos.x) / sqrt((playerPos.x - pos.x) * (playerPos.x - pos.x) + (playerPos.y - pos.y) * (playerPos.y - pos.y)));
        if (playerPos.y - pos.y < 0)
            degToPlayer += M_PI;
        SoundManager::playSound("se_tan00.wav");
        for (int i = -int(amount / 2); i <= int(amount / 2); i++)
            createBullet(pos.x, pos.y, 'E', 4, 50, degToPlayer + i * 0.1, 0, 50);
    }
}

void DanmakuManager::bulletUpdate(float dt)
{
    for (int i = 0; i < bullets.size(); i++)
    {
        if (bullets[i]->checkType() == 'B')
        {
            bullets[i]->sprite = sprites.BulletSprites;
            bullets[i]->sprite.setOrigin(8, 8);
            bullets[i]->sprite.setScale(1.4, 1.4);
            bullets[i]->sprite.setRotation(bullets[i]->rotation * 180 / M_PI + 90);
        }
        else if (bullets[i]->checkType() == 'b')
        {
            bullets[i]->sprite = sprites.BulletSprites1;
            bullets[i]->sprite.setOrigin(8, 8);
            bullets[i]->sprite.setScale(1.5, 1.5);
        }
        bullets[i]->process(dt);
    }
}

