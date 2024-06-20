#include "gameBoard.h"
std::vector<cords> adjacentTiles(int i, int j)
{
    int num = j * 6 + i;
    std::vector<cords> cors;
    cords temp;
    int i1 = num - 6;
    int i2 = num - 1;
    int i3 = num + 1;
    int i4 = num + 6;
    temp.x = i;
    temp.y = j;
    cors.push_back(temp);
    if (i1 >= 0)
    {
        temp.x = i1 % 6;
        temp.y = int(i1 / 6);
        cors.push_back(temp);
    }
    if ((i2 % 6) != 5 && i2 >= 0)
    {
        temp.x = i2 % 6;
        temp.y = int(i2 / 6);
        cors.push_back(temp);
    }
    if ((i3 % 6) != 0)
    {
        temp.x = i3 % 6;
        temp.y = int(i3 / 6);
        cors.push_back(temp);
    }
    if (i4 < 42)
    {
        temp.x = i4 % 6;
        temp.y = int(i4 / 6);
        cors.push_back(temp);
    }
    return cors;
}
Entity::Entity(float _x, float _y, float _hitbox_size, float _speed, float _rotation, float _ang_speed_of_rotation, float _acceleration, char _type)
{
    static int num = 0;
    num++;
    if (num > 1000)
        num = 2;
    this->destroyed = false;
    this->hitbox_size = _hitbox_size;
    if (_x == 0)
        this->x = GAME_WIDTH / 2;
    else
        this->x = _x;
    if (_y == 0)
        this->y = GAME_HEIGHT / 4;
    else
        this->y = _y;
    this->speed = _speed;
    this->ang_speed_of_rotation = _ang_speed_of_rotation;
    this->rotation = _rotation;
    this->acceleration = _acceleration;
    this->color = sf::Color::White;
    this->circHitbox.setRadius(hitbox_size);
    this->circHitbox.setOrigin(this->hitbox_size, this->hitbox_size);
    this->circHitbox.setPosition(this->x, this->y);
}
void Entity::process(float dt)
{
    this->rotation = this->rotation + dt * this->ang_speed_of_rotation;
    this->speed = this->speed + dt * this->acceleration;
    this->x = this->x + dt * this->speed * cos(this->rotation);
    this->y = this->y + dt * this->speed * sin(this->rotation);
    if (this->checkType() == 'B' || this->checkType() == 'b')
        this->sprite.setPosition(this->x + W_OFFSET, this->y +H_OFFSET);
    else
        this->sprite.setPosition(this->x  + W_OFFSET + (this->sprite.getScale().x / 4) * (this->sprite.getTexture())->getSize().x, this->y + H_OFFSET + (this->sprite.getScale().y / 4) * (this->sprite.getTexture())->getSize().y);
    this->circHitbox.setPosition(this->x + W_OFFSET, this->y + H_OFFSET);
    this->sprite.setColor(this->color);
    this->circHitbox.setFillColor(this->color);
    this->animTimer += dt;
}
cords Entity::getLocation()
{
    cords temp;
    temp.x = this->x;
    temp.y = this->y;
    return temp;
}
Points::Points(float _x, float _y, char _type, int _value)
:Entity(_x,_y, 15, -130, M_PI / 2, 0, 100)
{
    this->typeP = _type;
    this->value = _value;
    if (_type=='s')
        this->texture.loadFromFile("resources\\sprites\\scorePoint.png");
    else
        this->texture.loadFromFile("resources\\sprites\\powerPoint.png");
    this->texture.setSmooth(true);
    this->sprite.setTexture(this->texture);
    this->sprite.setScale(1.6, 1.6);
    this->circHitbox.setRadius(30);
    this->type = 'S';
}
void Points::destroy(Board *b, char full)
{
    if (!b->player->isDead())
    {
        b->playSound("se_item00.wav");
        if (this->getType() == 'p')
        {
            b->changeScores(50);
            if (b->player->power < 500)
                b->player->power++;
            if (!(b->player->power % 100))
                b->playSound("se_powerup.wav");
        }
        else if (this->getType() == 's')
            b->changeScores(this->getValue());
        this->destroyed = true;
    }
}
Bullet::Bullet(float _x, float _y, char _owner, float _hitbox_size, float _speed, float _rotation, float _ang_speed_of_rotation, float _acceleration)
    :Entity(_x,_y,_hitbox_size,_speed,_rotation,_ang_speed_of_rotation,_acceleration)
{
    this->owner = _owner;
    if (this->owner == 'E')
        this->type = 'B';
    else
        this->type = 'b';
}
Player::Player()
:Entity(GAME_WIDTH / 2, GAME_HEIGHT * 3 / 4, 7, 0, 0, 0, 0,'P')
{
    this->xSpeed = 0;
    this->ySpeed = 0;
    this->power = 0;
    this->cooldown = false;
    this->shooting = false;
    this->lives = 5;
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
    if (A&&D)
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
        if (this->lives > 0)
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
void Player::destroy(Board* b, char full)
{
    if (!this->cooldown)
    {
        b->playSound("se_pldead00.wav");
        this->cooldown = true;
        this->lives--;
        this->power /= 2;
        if (this->lives < 0)
            this->destroyed = true;
    }
}
void Player::doWhenReady(Board* b, float dt)
{
    if (!(this->isDead()) && this->shooting)
    {
        b->playSound("se_plst00.wav");
        if (this->power < 100)
            for (int i = -1; i < 2; i++)
                b->createBullet(this->x + 5 * i, this->y, 'P', 6, 1000, M_PI / 2 * 3 + 0.02 * i, 0, 100);
        else if (this->power < 200)
            for (int i = -2; i < 3; i++)
                b->createBullet(this->x + 5 * i, this->y, 'P', 6, 1000, M_PI / 2 * 3 + 0.02 * i, 0, 100);
        else if (this->power < 300)
            for (int i = -3; i < 4; i++)
                b->createBullet(this->x + 5 * i, this->y, 'P', 6, 1000, M_PI / 2 * 3 + 0.02 * i, 0, 100);
        else
            for (int i = -4; i < 5; i++)
                b->createBullet(this->x + 5 * i, this->y, 'P', 6, 1000, M_PI / 2 * 3 + 0.02 * i, 0, 100);
    }
}
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
void Enemy::destroy(Board* b, char full)
{
    
    this->hp = this->hp - 1;
    this->color = sf::Color::Red;
    if ((this->hp < 0 && !this->destroyed) || full=='f')
    {
        b->playSound("se_enep00.wav");
        int num = this->getSize() / 5;
        if (rand() % 5)
        {
            for (int i = 0; i < num; i++)
                b->createRandomScorePoints(this->getLocation());
        }
        b->changeScores(num * 100);
        this->destroyed = true;
    }
    else
        b->playSound("se_damage00.wav");
}
void Enemy::doWhenReady(Board* b, float dt)
{
    int type = this->getType();
    int variant = this->getVariant();
    if (!this->boss)
    {
        if (type < 3)
        {
            if (rand() % 5)
                b->CircularShot(this->getLocation(), 3, true, 10, dt);
        }
        else if (type == 3)
        {
            if (variant % 3 == 0)
                b->CircularShot(this->getLocation(), 10);
            else if (variant % 3 == 1)
                b->SnowflakeShot(this->getLocation(), 6);
            else if (variant % 3 == 2)
                b->HomingUShot(this->getLocation(), 7);
        }
        else if (type >= 4 && type < 6)
        {
            if (variant % 3 == 0)
                b->CircularShot(this->getLocation(), 10, true, 4, dt);
            else if (variant % 3 == 1)
                b->SnowflakeShot(this->getLocation(), 6, true, 4, dt);
            else if (variant % 3 == 2)
                b->HomingUShot(this->getLocation(), 7, true, 4, dt);
        }
    }
    else
    {
        if (variant == 0)
            b->EternalMeek(50, this->getLocation());
        else if (variant == 1)
            b->PsychadelicRings(8, this->getLocation(), dt);
        else if (variant == 2)
            b->GridSheet(this->getLocation(), dt, 24);
    }
}

Board::Board()
{
    endStage = false;
    switch1 = false;
    pause = false;
    stage_progress = 0;
    internalScore = 0;
    waveTimer = 0;
    waitingTimer = 0;
    player = new Player();
    entities = { this->player };
    score = 0;
    Animation reimu("reimu",1.6);
    AnimationTypes.push_back(reimu);
    Animation enemy("enemy", 1.6);
    AnimationTypes.push_back(enemy);
    BulletTextures.loadFromFile("resources\\sprites\\bullet1a.png");
    BulletTextures.setSmooth(true);
    BulletSprites.setTexture(this->BulletTextures);
    BulletTextures1.loadFromFile("resources\\sprites\\playerBullet1.png");
    BulletTextures1.setSmooth(true);
    BulletSprites1.setTexture(this->BulletTextures1);
    GameBackground.loadFromFile("resources\\HUD\\bg.png");
    GAMEBOARD.setTexture(GameBackground);
    GAMEBOARD.setPosition(sf::Vector2f(W_OFFSET, H_OFFSET));
    GAMEBOARD.setScale(3, 3.5);
    HUDt.loadFromFile("resources\\HUD\\bgHUD.png");
    HUD.setTexture(HUDt);
    HUD.setScale(1.875, 1.875);
    MainMenuT.loadFromFile("resources\\HUD\\MainMenu.png");
    MainMenu.setTexture(MainMenuT);
    MainMenu.setScale(2, 2);
    GameBackground.setSmooth(true);
    HUDt.setSmooth(true);
    MainMenuT.setSmooth(true);
    sf::Text option1;
    option1.setString("New Game");
    option1.setPosition(70,30);
    sf::Text option2;
    option2.setString("Spellcard Test");
    option2.setPosition(220, 30);
    Options.push_back(option1);
    Options.push_back(option2);
}
void Board::createBullet(float _x, float _y, char owner, float _hitbox_size, float _speed, float _rotation, float _ang_speed_of_rotation, float _acceleration)
{
    Bullet *temp = new Bullet(_x, _y, owner, _hitbox_size, _speed, _rotation, _ang_speed_of_rotation, _acceleration);
    this->entities.insert(entities.end(), temp);
}
void Board::EternalMeek(int difficulty, cords pos)
{
    if (!(rand() % difficulty))
    {
        this->playSound("se_tan00.wav");
        this->createBullet(pos.x, pos.y, 'E', 4, (250 + round(250 * rand() % 2)), (float)rand() / (float)RAND_MAX * (2 * M_PI), 0, 10);
    }
}
void Board::PsychadelicRings(int difficulty, cords pos, float dt)
{
    static float time_elapsed = 0;
    static bool ver = false;
    time_elapsed += dt;
    float dif = float(rand() % 3) / 15.f;
    if (time_elapsed > 0.5)
    {
        int amount = (difficulty-8)/4;
        this->playSound("se_tan00.wav");
        time_elapsed = 0;
        ver = !ver;
        for (int i = 0; i < difficulty; i++)
            this->createBullet(pos.x + 2, pos.y + 2, 'E', 4, 10, 2 * M_PI / difficulty * i, (1 - (2*ver))*(0.5 + dif), 50);
        if (amount > 0)
        {
            float degToPlayer = acos((this->player->getLocation().x - pos.x) / sqrt((this->player->getLocation().x - pos.x) * (this->player->getLocation().x - pos.x) + (this->player->getLocation().y - pos.y) * (this->player->getLocation().y - pos.y)));
            if (this->player->getLocation().y - pos.y < 0)
                degToPlayer += M_PI;
            for (int i = -int(amount / 2); i <= int(amount / 2); i++)
                this->createBullet(pos.x, pos.y, 'E', 4, 10, degToPlayer + i * 0.1, 0, 80);
        }
    }
}
void Board::GridSheet(cords pos, float dt, int difficulty)
{
	static float time_elapsed = 0;
	time_elapsed += dt;
    float newXL = pos.x - time_elapsed * 300;
    float newXR = pos.x + time_elapsed * 300;
    if (!(int(time_elapsed * 200) % 70) && newXL > 0)
    {
        this->playSound("se_tan00.wav");
        for (int i = 0; i < difficulty; i++)
            this->createBullet(newXR, pos.y, 'E', 4, 10, 2 * M_PI / difficulty * i, 0, 50);
        for (int i = 0; i < difficulty; i++)
            this->createBullet(newXL, pos.y, 'E', 4, 10, 2 * M_PI / difficulty * i, 0, 50);
    }
    if (time_elapsed > 3)
        time_elapsed = 0;
}
void Board::CircularShot(cords pos, int amount, bool timed, float T, float dt)
{
    if (timed)
    {
        static float time_elapsed = 0;
        time_elapsed += dt;
        if (time_elapsed > T)
        {
            this->playSound("se_tan00.wav");
            time_elapsed = 0;
            for (int i = 0; i < amount; i++)
                this->createBullet(pos.x + 2, pos.y + 2, 'E', 4, 10, 2 * M_PI / amount * i, 0, 50);
        }
    }
    else
    {
        this->playSound("se_tan00.wav");
        for (int i = 0; i < amount; i++)
            this->createBullet(pos.x + 2, pos.y + 2, 'E', 4, 10, 2 * M_PI / amount * i, 0, 50);
    }
}
void Board::SnowflakeShot(cords pos, int amount, bool timed, float T, float dt)
{
    if (timed)
    {
        static float time_elapsed = 0;
        time_elapsed += dt;
        if (time_elapsed > T)
        {
            this->playSound("se_tan00.wav");
            time_elapsed = 0;
            for (int j = 1; j < 5; j++)
            {
                for (int i = 0; i < amount; i++)
                    this->createBullet(pos.x + 2, pos.y + 2, 'E', 4, 10, 2 * M_PI / amount * i, 0, j * 30);
            }
        }
    }
    else
    {
        this->playSound("se_tan00.wav");
        for (int j = 1; j < 5; j++)
        {
            for (int i = 0; i < amount; i++)
                this->createBullet(pos.x + 2, pos.y + 2, 'E', 4, 10, 2 * M_PI / amount * i, 0, j * 30);
        }
    }
}
void Board::HomingUShot(cords pos, int amount, bool timed, float T, float dt)
{
    if (timed)
    {
        static float time_elapsed = 0;
        time_elapsed += dt;
        if (time_elapsed > T)
        {
            this->playSound("se_tan00.wav");
            time_elapsed = 0;
            float degToPlayer = acos((this->player->getLocation().x - pos.x) / sqrt((this->player->getLocation().x - pos.x) * (this->player->getLocation().x - pos.x) + (this->player->getLocation().y - pos.y) * (this->player->getLocation().y - pos.y)));
            if (this->player->getLocation().y - pos.y < 0)
                degToPlayer += M_PI;

            for (int i = -int(amount / 2); i <= int(amount / 2); i++)
                this->createBullet(pos.x, pos.y, 'E', 4, 50, degToPlayer + i * 0.1, 0, 50);
        }
    }
    else
    {
        float degToPlayer = acos((this->player->getLocation().x - pos.x) / sqrt((this->player->getLocation().x - pos.x) * (this->player->getLocation().x - pos.x) + (this->player->getLocation().y - pos.y) * (this->player->getLocation().y - pos.y)));
        if (this->player->getLocation().y - pos.y < 0)
            degToPlayer += M_PI;
        this->playSound("se_tan00.wav");
        for (int i = -int(amount / 2); i <= int(amount / 2); i++)
            this->createBullet(pos.x, pos.y, 'E',4 , 50, degToPlayer + i * 0.1, 0, 50);
    }
}
bool Board::checkCollisionTypes(Entity * a1, Entity * b1)
{
    char a = a1->checkType();
    char b = b1->checkType();
    int numA =0;
    int numB =0;
    char toNum[5] = {'E','P','S','b','B'};
    char cols[5][5] = { {0,0,0,1,0},{1,0,0,0,1},{0,1,0,0,0},{1,0,0,0,0},{0,1,0,0,0} };
    for (int k = 0;k<5;k++)
    {
        if (a == toNum[k])
            numA = k;
        if (b == toNum[k])
            numB = k;
    }
    return cols[numA][numB];
}
void Board::manageCollisions()
{
    std::vector <Entity*> areas[6][7];
    for (auto i : this->entities)
        if ((i->getLocation().x / 120)<6 && (i->getLocation().y / 120)<7 && (i->getLocation().x / 120) > 0 && (i->getLocation().y / 120) > 0)
            areas[int(i->getLocation().x/120)][int(i->getLocation().y / 120)].insert(areas[int(i->getLocation().x / 120)][int(i->getLocation().y / 120)].end(),i);
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            for (auto k : areas[i][j])
            {
                if (k->checkType() == 'B')
                {
                    if (k->getHitboxC().getGlobalBounds().intersects(entities[0]->getHitboxC().getGlobalBounds()))
                    {
                        k->destroy(this);
                        entities[0]->destroy(this);
                    }
                }
                else if (k->checkType() == 'S')
                {
                    if (k->getHitboxC().getGlobalBounds().intersects(entities[0]->getHitboxC().getGlobalBounds()))
                    {
                        k->destroy(this);
                    }
                }
                else
                {
                    auto temp = adjacentTiles(i, j);
                    for (auto l : temp)
                    {
                        for (auto m : areas[int(l.x)][int(l.y)])
                        {
                            if (k->getHitboxC().getGlobalBounds().intersects(m->getHitboxC().getGlobalBounds()))
                            {
                                if (this->checkCollisionTypes(m, k))
                                    m->destroy(this);
                            }
                        }
                    }
                }
            }
        }
    }
}
void Board::changeScores(int value)
{
    this->internalScore += value;
    this->score += value;
}
void Board::createRandomScorePoints(cords pos)
{
    char _type;
    int _value;
    if (rand() % 2)
    {
        _type = 'p';
        _value = 1;
    }
    else
    {
        _type = 's';
        _value = 1000;
    }
    Points *temp = new Points(pos.x + ((rand() % 20) * 2 * pow(-1, rand() % 2)), pos.y + ((rand() % 20) * 2 * pow(-1, rand() % 2)), _type, _value);
    this->entities.insert(this->entities.end(), temp);
}
void Board::updateEntities(float dt)
{
    if (this->internalScore >= 100000)
    {
        this->internalScore -= 100000;
        this->player->gainLife();
        this->playSound("se_extend.wav");
    }
    this->manageCollisions();
    for (int i = 0; i < this->entities.size(); i++)
    {
        if (this->entities[i]->out_of_bounds() || this->entities[i]->destroyed)
            this->entities.erase(this->entities.begin() + i);
        else
        {
            if (this->entities[i]->checkType() == 'P')
            {
                if (this->player->movingDirection() == 'L')
                {
                    this->AnimationTypes[0].left.play(dt);
                    this->entities[i]->sprite = this->AnimationTypes[0].left.baseSprite;
                }
                else if (this->player->movingDirection() == 'R')
                {
                    this->AnimationTypes[0].right.play(dt);
                    this->entities[i]->sprite = this->AnimationTypes[0].right.baseSprite;
                }
                else
                {
                    AnimationTypes[0].left.reset();
                    AnimationTypes[0].right.reset();
                    this->AnimationTypes[0].idle.play(dt);
                    this->entities[i]->sprite = this->AnimationTypes[0].idle.baseSprite;
                }
            }
            else if (this->entities[i]->checkType() == 'B')
            {
                this->entities[i]->sprite = this->BulletSprites;
                this->entities[i]->sprite.setOrigin(8, 8);
                this->entities[i]->sprite.setScale(1.4, 1.4);
                this->entities[i]->sprite.setRotation(this->entities[i]->rotation * 180 / M_PI + 90);
            }
            else if (this->entities[i]->checkType() == 'b')
            {
                this->entities[i]->sprite = this->BulletSprites1;
                this->entities[i]->sprite.setOrigin(8, 8);
                this->entities[i]->sprite.setScale(1.5, 1.5);
            }
            else if (this->entities[i]->checkType()!= 'S')
            {
                this->AnimationTypes[1].idle.playE(this->entities[i]->animTimer);
                this->entities[i]->sprite = this->AnimationTypes[1].idle.baseSprite;
                this->entities[i]->sprite.setOrigin(24, 24);
            }
            this->entities[i]->process(dt);
        }
        if (this->entities[i]->isReady())
        {
            this->entities[i]->readyChange(false);
            this->entities[i]->doWhenReady(this, dt);
        }
    }
    std::string temp = "Score: \n" + std::to_string(this->score);
    this->Score.setString(temp);
    temp = "Power: \n" + std::to_string(this->player->power);
    this->Power.setString(temp);
    temp = "Lives: \n" + std::to_string(this->player->numberOfLives());
    this->Lives.setString(temp);
    this->Score.setPosition((APP_WIDTH - GAME_WIDTH) / 5 + GAME_WIDTH + W_OFFSET, APP_HEIGHT / 8);
    this->Power.setPosition((APP_WIDTH - GAME_WIDTH) / 5 + GAME_WIDTH + W_OFFSET, APP_HEIGHT / 8 + 100);
    this->Lives.setPosition((APP_WIDTH - GAME_WIDTH) / 5 + GAME_WIDTH + W_OFFSET, APP_HEIGHT / 8 + 200);
}
void Board::playSound(std::string filename)
{
	for (int i = 0; i < 10; i++)
		if (voices[i].getStatus() != sf::SoundSource::Status::Playing)
		{
            buffers[i].loadFromFile("resources\\sfx\\" + filename);
			voices[i].setBuffer(buffers[i]);
			voices[i].play();
            break;
		}
}
void Board::createEnemy(bool _boss, int _type, int _variant, int _position, float _x, float _y, float _hp, float _hitbox_radius, float _speed, float _rotation, float _ang_speed_of_rotation, float _acceleration)
{
    static int count = 0;
    count++;
    Enemy *temp = new Enemy(_boss, _type, _variant, _position, _x, _y, _hp, _hitbox_radius, _speed, _rotation, _ang_speed_of_rotation, _acceleration);
    this->entities.insert(entities.end(), temp);
}
void Board::enemiesInWaves(int type, int variant)
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
            for (int i =0; i<5;i++)
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
void Board::waveProcess(float dt)
{
    static int type = (rand() % ENEMY_TYPES) + 1;
    static int variant;
    if (type == 1)
        variant = 0;
    else if (type == 2)
        variant = rand() % 2;
    else if (type == 3)
        variant = rand() % 6;
    else if (type > 3 && type < 6)
        variant = rand() % 3;
    static float breaktime;
    if (type == 1)
        breaktime = ((float)(rand() % 3) + 1) / (float)2;
    else if (type == 2)
        breaktime = ((float)(rand() % 3) + 1) / (float)3;
    else if (type == 3)
        breaktime = 5;
    else if (type == 4)
        breaktime = 9;
    else if (type == 5)
        breaktime = 5;
    static float time;
    if (type == 1)
        time = 20 + (rand() % 3 * 10);
    else if (type == 2)
        time = 15 + (rand() % 3 * 10);
    else if (type == 3)
        time = 20 + (rand() % 3 * 10);
    else if (type >= 4)
        time = 10 + (rand() % 3 * 10);
    this->waveTimer += dt;
    this->waitingTimer += dt;
    if (this->waitingTimer > breaktime && this->waveTimer < time && this->waveTimer > 0)
    {
        this->waitingTimer = 0;
        enemiesInWaves(type, variant);
    }
    if (this->waveTimer > time)
    {
        this->waveTimer = -8;
        this->switch1 = true;
        type = (rand() % ENEMY_TYPES) + 1;
        if (type == 1)
            variant = 0;
        else if (type == 2)
            variant = rand() % 2;
        else if (type == 3)
            variant = rand() % 6;
        else if (type > 3 && type < 6)
            variant = rand() % 3;
        else if (type > 5)
            variant = rand() % 7;
        if (type == 1)
            breaktime = ((float)(rand() % 3) + 1) / (float)2;
        else if (type == 2)
            breaktime = ((float)(rand() % 3) + 1) / (float)3;
        else if (type == 3)
            breaktime = 5;
        else if (type == 4)
            breaktime = 9;
        else if (type == 5)
            breaktime = 15;
        if (type == 1)
            time = 20 + (rand() % 3 * 10);
        else if (type == 2)
            time = 15 + (rand() % 3 * 10);
        else if (type == 3)
            time = 20 + (rand() % 3 * 10);
        else if (type >= 4)
            time = 30 + (rand() % 3 * 10);
    }
}
void Board::gameProcess(float dt, int gameMode)
{
    if (!this->pause && !this->endStage)
    {
        if (this->player->numberOfLives() == 0)
            this->pause = true;
        this->updateEntities(dt);
        static bool on = true;
        static float timer = 0;
        if (gameMode == 2)
        {
            if (on)
            {
                on = false;
                this->createEnemy(true, 1, 2);
            }
        }
        else
        {
            static int numWaves = 7 + rand() % 5;
            if (this->switch1)
            {
                this->stage_progress += 1.f / (float)numWaves;
                this->switch1 = false;
            }
            if (this->stage_progress < 0.45 || (this->stage_progress > 0.65 && this->stage_progress < 1))
                this->waveProcess(dt);
            else if (this->stage_progress >= 1)
            {
                timer += dt;
                if (timer <= 60 && on && timer > 10)
                {
                    on = false;
                    for (int j = 0; j < this->entities.size(); j++)
                        if (this->entities[j]->type == 'E')
                            this->entities[j]->destroy(this, 'f');
                    int random = rand() % MIDBOSS_TYPES;
                    int random1 = rand() % SPELLCARDS;
                    createEnemy(true, random+1, random1, 0, GAME_WIDTH / 2 - 30, 30, 1000, 30, 50, M_PI / 2, 0, -10);
                }
                else if (timer > 60)
                {
                    for (int j = 0; j < this->entities.size(); j++)
                        if (this->entities[j]->type == 'E')
                            this->entities[j]->destroy(this, 'f');
                    this->endStage = true;
                    on = true;
                }
            }
            else
            {
                timer += dt;
                if (timer <= 30 && on)
                {
                    on = false;
                    for (int j = 0; j < this->entities.size(); j++)
                        if (this->entities[j]->type == 'E')
                            this->entities[j]->destroy(this, 'f');
                    int random = rand() % MIDBOSS_TYPES;
                    int random1 = rand() % SPELLCARDS;
                    createEnemy(true, random+1, random1, 0, GAME_WIDTH / 2 - 30, 30, 1000, 30, 50, M_PI / 2, 0, -10);
                }
                else if (timer > 30 || this->entities.size() == 0)
                {
                    for (int j = 0; j < this->entities.size(); j++)
                        if (this->entities[j]->type == 'E')
                            this->entities[j]->destroy(this, 'f');
                    this->stage_progress = 0.66;
                    timer = 0;
                    on = true;
                }
            }
        }
    }
}
void Board::restart()
{
    switch1 = false;
    pause = false;
    stage_progress = 0;
    internalScore = 0;
    waveTimer = 0;
    waitingTimer = 0;
    player = new Player();
    entities = { this->player };
    score = 0;
}
