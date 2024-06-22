#include "board.h"

Board::Board()
{
    endStage = false;
    switch1 = false;
    pause = false;
    stage_progress = 0;
    waveTimer = 0;
    waitingTimer = 0;
    player = new Player();
    entities = { this->player };
    Animation reimu("reimu", 1.6);
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
    option1.setPosition(70, 30);
    sf::Text option2;
    option2.setString("Spellcard Test");
    option2.setPosition(220, 30);
    Options.push_back(option1);
    Options.push_back(option2);
}

bool Board::checkCollisionTypes(Entity* a1, Entity* b1)
{
    char a = a1->checkType();
    char b = b1->checkType();
    int numA = 0;
    int numB = 0;
    char toNum[5] = { 'E','P','S','b','B' };
    char cols[5][5] = { {0,0,0,1,0},{1,0,0,0,1},{0,1,0,0,0},{1,0,0,0,0},{0,1,0,0,0} };
    for (int k = 0; k < 5; k++)
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
        if ((i->getLocation().x / 120) < 6 && (i->getLocation().y / 120) < 7 && (i->getLocation().x / 120) > 0 && (i->getLocation().y / 120) > 0)
            areas[int(i->getLocation().x / 120)][int(i->getLocation().y / 120)].insert(areas[int(i->getLocation().x / 120)][int(i->getLocation().y / 120)].end(), i);
    for (auto i : DanmakuManager::bullets)
    {
        if ((i->getLocation().x / 120) < 6 && (i->getLocation().y / 120) < 7 && (i->getLocation().x / 120) > 0 && (i->getLocation().y / 120) > 0)
            areas[int(i->getLocation().x / 120)][int(i->getLocation().y / 120)].insert(areas[int(i->getLocation().x / 120)][int(i->getLocation().y / 120)].end(), i);
    }
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
                        k->destroy();
                        entities[0]->destroy();
                    }
                }
                else if (k->checkType() == 'S')
                {
                    if (k->getHitboxC().getGlobalBounds().intersects(entities[0]->getHitboxC().getGlobalBounds()))
                    {
                        k->destroy();
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
                                    m->destroy();
                            }
                        }
                    }
                }
            }
        }
    }
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
    Points* temp = new Points(pos.x + ((rand() % 20) * 2 * pow(-1, rand() % 2)), pos.y + ((rand() % 20) * 2 * pow(-1, rand() % 2)), _type, _value);
    this->entities.insert(this->entities.end(), temp);
}
void Board::updateEntities(float dt)
{
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
            else if (this->entities[i]->checkType() != 'S')
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
            this->entities[i]->doWhenReady(dt);
        }
    }


    for (int i = 0; i < DanmakuManager::bullets.size(); i++)
    {
                    if (DanmakuManager::bullets[i]->checkType() == 'B')
                    {
                        DanmakuManager::bullets[i]->sprite = this->BulletSprites;
                        DanmakuManager::bullets[i]->sprite.setOrigin(8, 8);
                        DanmakuManager::bullets[i]->sprite.setScale(1.4, 1.4);
                        DanmakuManager::bullets[i]->sprite.setRotation(DanmakuManager::bullets[i]->rotation * 180 / M_PI + 90);
            }
                    else if (DanmakuManager::bullets[i]->checkType() == 'b')
                    {
                        DanmakuManager::bullets[i]->sprite = this->BulletSprites1;
                        DanmakuManager::bullets[i]->sprite.setOrigin(8, 8);
                        DanmakuManager::bullets[i]->sprite.setScale(1.5, 1.5);
            }
    }
    std::string temp = "Score: \n" + std::to_string(ScoreManager::getScore());
    this->Score.setString(temp);
    temp = "Power: \n" + std::to_string(ScoreManager::getPower());
    this->Power.setString(temp);
    temp = "Lives: \n" + std::to_string(ScoreManager::getLives());
    this->Lives.setString(temp);
    this->Score.setPosition((APP_WIDTH - GAME_WIDTH) / 5 + GAME_WIDTH + W_OFFSET, APP_HEIGHT / 8);
    this->Power.setPosition((APP_WIDTH - GAME_WIDTH) / 5 + GAME_WIDTH + W_OFFSET, APP_HEIGHT / 8 + 100);
    this->Lives.setPosition((APP_WIDTH - GAME_WIDTH) / 5 + GAME_WIDTH + W_OFFSET, APP_HEIGHT / 8 + 200);
}

void Board::createEnemy(bool _boss, int _type, int _variant, int _position, float _x, float _y, float _hp, float _hitbox_radius, float _speed, float _rotation, float _ang_speed_of_rotation, float _acceleration)
{
    static int count = 0;
    count++;
    Enemy* temp = new Enemy(_boss, _type, _variant, _position, _x, _y, _hp, _hitbox_radius, _speed, _rotation, _ang_speed_of_rotation, _acceleration);
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
        if (ScoreManager::getLives() == 0)
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
                            this->entities[j]->destroy('f');
                    int random = rand() % MIDBOSS_TYPES;
                    int random1 = rand() % SPELLCARDS;
                    createEnemy(true, random + 1, random1, 0, GAME_WIDTH / 2 - 30, 30, 1000, 30, 50, M_PI / 2, 0, -10);
                }
                else if (timer > 60)
                {
                    for (int j = 0; j < this->entities.size(); j++)
                        if (this->entities[j]->type == 'E')
                            this->entities[j]->destroy('f');
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
                            this->entities[j]->destroy('f');
                    int random = rand() % MIDBOSS_TYPES;
                    int random1 = rand() % SPELLCARDS;
                    createEnemy(true, random + 1, random1, 0, GAME_WIDTH / 2 - 30, 30, 1000, 30, 50, M_PI / 2, 0, -10);
                }
                else if (timer > 30 || this->entities.size() == 0)
                {
                    for (int j = 0; j < this->entities.size(); j++)
                        if (this->entities[j]->type == 'E')
                            this->entities[j]->destroy('f');
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
    ScoreManager::reset();
    waveTimer = 0;
    waitingTimer = 0;
    player = new Player();
    entities = { this->player };
}
