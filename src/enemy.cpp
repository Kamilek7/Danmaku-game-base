#include "enemy.h"

Enemy::Enemy(bool _boss, int _type, int _variant, int _position, float _x, float _y, float _hp, float _hitbox_size, float _speed, float _rotation, float _ang_speed_of_rotation, float _acceleration)
    :Entity(_x, _y, _hitbox_size, _speed, _rotation, _ang_speed_of_rotation, _acceleration, 'E')
{
    this->boss = _boss;
    this->animTimer = 0;
    this->variant = _variant;
    this->change = true;
    this->position = _position;
    this->timer = 0;
    this->type = 'E';
    this->type_ = _type;
    this->hp = _hp;
}
void Enemy::process(float dt)
{
    if (!this->boss)
    {
        if (this->type_ == 1)
        {
            this->ready = true;
            this->timer += dt;
            static float initial_speed;
            if (this->timer < 1)
                initial_speed = this->speed;
            if (this->timer >= GAME_WIDTH / 4 / initial_speed && this->timer < (GAME_WIDTH / 4 / initial_speed) + 1)
            {
                if (this->x > GAME_WIDTH / 2)
                    this->ang_speed_of_rotation = -M_PI / 2;
                else
                    this->ang_speed_of_rotation = M_PI / 2;
                if (this->position == 1)
                    this->speed = initial_speed + 3 * this->hitbox_size * M_PI / 2;
            }
            else
            {
                this->ang_speed_of_rotation = 0;
                this->speed = initial_speed;
            }
        }
        else if (this->type_ == 2)
            this->readyChange(true);
        else if (this->type_ == 3)
        {
            this->timer += dt;
            static float initial_speed;
            if (this->timer < 1)
                initial_speed = this->speed;
            if (this->timer >= 150 / initial_speed)
            {
                if (this->timer >= 375 / initial_speed)
                    this->timer = 150 / initial_speed;
                else if (this->timer >= 300 / initial_speed)
                {
                    this->ready = false;
                    this->acceleration = (initial_speed / (75 / initial_speed));
                    if (this->change)
                    {
                        this->ready = true;
                        float _rotation = (float)(rand() % 8) / (float)(8) * 2 * M_PI;
                        this->rotation = _rotation;
                        this->change = false;
                    }
                }
                else if (this->timer >= 225 / initial_speed)
                {
                    this->acceleration = 0;
                }
                else
                {
                    this->acceleration = -(initial_speed / (75 / initial_speed));
                    this->change = true;
                }
            }
            else
            {
                this->ang_speed_of_rotation = 0;
                this->speed = initial_speed;
            }
        }
        else if (this->type_ == 4)
            this->ready = true;
        else if (this->type_ == 5)
        {
            this->ready = true;
            this->timer += dt;
            if (this->timer >= 2.5)
            {
                this->timer = 0;
                this->ang_speed_of_rotation = -this->ang_speed_of_rotation;
            }
        }
    }
    else
    {
        if (this->type_ == 1)
        {
            if (this->speed <= 0)
            {
                this->speed = 0;
                this->acceleration = 0;
                this->ready = true;
            }
        }
    }
    static float indep_timer = 0;
    indep_timer += dt;
    if (indep_timer > 1000)
        indep_timer = 0;
    if (!(int(indep_timer * 10) % 3))
        this->color = sf::Color::White;
    this->Entity::process(dt);
}
void Enemy::destroy( char full)
{

    this->hp = this->hp - 1;
    this->color = sf::Color::Red;
    if ((this->hp < 0 && !this->destroyed) || full == 'f')
    {
        SoundManager::playSound("se_enep00.wav");
        int num = this->getSize() / 5;
        if (rand() % 5)
        {
            for (int i = 0; i < num; i++)
                PointsManager::createRandomScorePoints(this->getLocation());
        }
        ScoreManager::changeScores(num * 100);
        this->destroyed = true;
    }
    else
        SoundManager::playSound("se_damage00.wav");
}
void Enemy::doWhenReady(float dt)
{
    int type = this->getType();
    int variant = this->getVariant();
    if (!this->boss)
    {
        if (type < 3)
        {
            if (rand() % 5)
                DanmakuManager::CircularShot(this->getLocation(), 3, true, 10, dt);
        }
        else if (type == 3)
        {
            if (variant % 3 == 0)
                DanmakuManager::CircularShot(this->getLocation(), 10);
            else if (variant % 3 == 1)
                DanmakuManager::SnowflakeShot(this->getLocation(), 6);
            else if (variant % 3 == 2)
                DanmakuManager::HomingUShot(this->getLocation(), 7);
        }
        else if (type >= 4 && type < 6)
        {
            if (variant % 3 == 0)
                DanmakuManager::CircularShot(this->getLocation(), 10, true, 4, dt);
            else if (variant % 3 == 1)
                DanmakuManager::SnowflakeShot(this->getLocation(), 6, true, 4, dt);
            else if (variant % 3 == 2)
                DanmakuManager::HomingUShot(this->getLocation(), 7, true, 4, dt);
        }
    }
    else
    {
        if (variant == 0)
            DanmakuManager::EternalMeek(50, this->getLocation());
        else if (variant == 1)
            DanmakuManager::PsychadelicRings(8, this->getLocation(), dt);
        else if (variant == 2)
            DanmakuManager::GridSheet(this->getLocation(), dt, 24);
    }
}