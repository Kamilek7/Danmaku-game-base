#include "player.h"

Player::Player()
    :Entity(GAME_WIDTH / 2, GAME_HEIGHT * 3 / 4, 7, 0, 0, 0, 0, 'P')
{
    this->xSpeed = 0;
    this->ySpeed = 0;
    this->cooldown = false;
    this->shooting = false;
    this->type = 'P';
}
bool Player::isDead()
{
    if (this->cooldown)
        return true;
    else
        return false;
}
void Player::moveX(bool A, bool D)
{
    if (A)
        this->xSpeed = -SPEED;
    else if (D)
        this->xSpeed = SPEED;
    else
        this->xSpeed = 0;
    if (A && D)
        this->xSpeed = 0;
}
void Player::moveY(bool W, bool S)
{
    if (W)
        this->ySpeed = -SPEED;
    else if (S)
        this->ySpeed = SPEED;
    else
        this->ySpeed = 0;
    if (W && S)
        this->ySpeed = 0;
}
void Player::process(float dt)
{
    static float time1 = 0;
    time1 += dt;
    if (time1 > 0.1)
    {
        time1 = 0;
        this->readyChange(true);
    }
    else if (this->isReady())
        this->readyChange(false);
    this->x = this->x + dt * this->xSpeed;
    this->y = this->y + dt * this->ySpeed;
    if (this->x > GAME_WIDTH)
        this->x = GAME_WIDTH;
    else if (this->x < 0)
        this->x = 0;
    if (this->y > GAME_HEIGHT)
        this->y = GAME_HEIGHT;
    else if (this->y < 0)
        this->y = 0;
    static float time = 0;
    if (!this->isDead())
        this->color = sf::Color(255, 255, 255, 255);
    if (this->isDead())
    {
        time += dt;
        this->color = sf::Color(0, 255, 0, 0);
        if (ScoreManager::getLives() > 0)
        {
            if (time >= 3 && time < 3.1)
            {
                this->x = GAME_WIDTH / 2;
                this->y = GAME_HEIGHT * 3 / 4;
                this->color = sf::Color(255, 255, 255, 150);
            }
            if (time > 3 && time < 5)
            {
                float temp = time * 5 / 4 - int(time * 5 / 4);
                if (temp >= 0 && temp < 0.25)
                    this->color = sf::Color(0, 255, 0, 0);
                else
                    this->color = sf::Color(255, 255, 255, 150);
            }
            if (time >= 5)
            {
                time = 0;
                this->cooldown = false;
            }
        }
    }
    this->sprite.setPosition(this->x + W_OFFSET - (this->sprite.getTexture())->getSize().x * this->sprite.getScale().x / 2, this->y + H_OFFSET - (this->sprite.getTexture())->getSize().y * this->sprite.getScale().y / 2);
    this->circHitbox.setPosition(this->x + W_OFFSET, this->y + H_OFFSET);
    this->sprite.setColor(this->color);
}
void Player::destroy(char full)
{
    if (!this->cooldown)
    {
        SoundManager::playSound("se_pldead00.wav");
        this->cooldown = true;
        ScoreManager::changeLives(-1);
        ScoreManager::cutPower();
        if (ScoreManager::getLives() < 0)
            this->destroyed = true;
    }
}
void Player::doWhenReady(float dt)
{
    if (!(this->isDead()) && this->shooting)
    {
        SoundManager::playSound("se_plst00.wav");
        int tempPower = ScoreManager::getPower();
        if (tempPower < 50)
            for (int i = -1; i < 2; i++)
                DanmakuManager::createBullet(this->x + 5 * i, this->y, 'P', 6, 1000, M_PI / 2 * 3 + 0.02 * i, 0, 100);
        else if (tempPower < 100)
            for (int i = -2; i < 3; i++)
                DanmakuManager::createBullet(this->x + 5 * i, this->y, 'P', 6, 1000, M_PI / 2 * 3 + 0.02 * i, 0, 100);
        else if (tempPower < 150)
            for (int i = -3; i < 4; i++)
                DanmakuManager::createBullet(this->x + 5 * i, this->y, 'P', 6, 1000, M_PI / 2 * 3 + 0.02 * i, 0, 100);
        else
            for (int i = -4; i < 5; i++)
                DanmakuManager::createBullet(this->x + 5 * i, this->y, 'P', 6, 1000, M_PI / 2 * 3 + 0.02 * i, 0, 100);
    }
}