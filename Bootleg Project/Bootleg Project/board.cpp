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
        char cols[5][5] = { {0,0,0,1,0},{1,0,0,0,1},{0,0,0,0,0},{1,0,0,0,0},{0,1,0,0,0} };
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
    for (auto i : DanmakuManager::bullets)
    {
        if ((i->getLocation().x / 120) < 6 && (i->getLocation().y / 120) < 7 && (i->getLocation().x / 120) > 0 && (i->getLocation().y / 120) > 0)
            areas[int(i->getLocation().x / 120)][int(i->getLocation().y / 120)].insert(areas[int(i->getLocation().x / 120)][int(i->getLocation().y / 120)].end(), i);
    }
    for (auto i : PointsManager::points)
    {
        if ((i->getLocation().x / 120) < 6 && (i->getLocation().y / 120) < 7 && (i->getLocation().x / 120) > 0 && (i->getLocation().y / 120) > 0)
            areas[int(i->getLocation().x / 120)][int(i->getLocation().y / 120)].insert(areas[int(i->getLocation().x / 120)][int(i->getLocation().y / 120)].end(), i);
    }
    for (auto i : EnemyManager::enemies)
    {
        if ((i->getLocation().x / 120) < 6 && (i->getLocation().y / 120) < 7 && (i->getLocation().x / 120) > 0 && (i->getLocation().y / 120) > 0)
            areas[int(i->getLocation().x / 120)][int(i->getLocation().y / 120)].insert(areas[int(i->getLocation().x / 120)][int(i->getLocation().y / 120)].end(), i);
    }
	if ((this->player->getLocation().x / 120) < 6 && (this->player->getLocation().y / 120) < 7 && (this->player->getLocation().x / 120) > 0 && (this->player->getLocation().y / 120) > 0)
		areas[int(this->player->getLocation().x / 120)][int(this->player->getLocation().y / 120)].insert(areas[int(this->player->getLocation().x / 120)][int(this->player->getLocation().y / 120)].end(), this->player);

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            for (auto k : areas[i][j])
            {
                if (!k->isDestroyed() && !this->player->isDestroyed() && !this->player->isInCoolDown() && !k->isInCoolDown())
                {
                    if (k->checkType() == 'S')
                    {
                        if (k->getHitboxC().getGlobalBounds().intersects(this->player->getHitboxC().getGlobalBounds()))
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
                                if (!m->isDestroyed() && k->getHitboxC().getGlobalBounds().intersects(m->getHitboxC().getGlobalBounds()) && !m->isInCoolDown() )
                                {
                                    if (this->checkCollisionTypes(m, k))
                                    {
                                        m->destroy();
                                        k->destroy();
                                    }

                                }
                            }
                        }
                    }
                }

            }
        }
    }
}
void Board::updateEntities(float dt)
{
    this->manageCollisions();
    this->player->playerUpdate(dt);
    DanmakuManager::setPlayerPos(this->player->getLocation());
    DanmakuManager::bulletUpdate(dt);
    PointsManager::pointsUpdate(dt);
    EnemyManager::enemyUpdate(dt);
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
        EnemyManager::enemiesInWaves(type, variant);
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
                EnemyManager::createEnemy(true, 1, 2);
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
                    for (int j = 0; j < EnemyManager::enemies.size(); j++)
                        EnemyManager::enemies[j]->destroy('f');
                    int random = rand() % MIDBOSS_TYPES;
                    int random1 = rand() % SPELLCARDS;
                    EnemyManager::createEnemy(true, random + 1, random1, 0, GAME_WIDTH / 2 - 30, 30, 1000, 30, 50, M_PI / 2, 0, -10);
                }
                else if (timer > 60)
                {
                    for (int j = 0; j < EnemyManager::enemies.size(); j++)
                        EnemyManager::enemies[j]->destroy('f');
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
                    for (int j = 0; j < EnemyManager::enemies.size(); j++)
                        EnemyManager::enemies[j]->destroy('f');
                    int random = rand() % MIDBOSS_TYPES;
                    int random1 = rand() % SPELLCARDS;
                    EnemyManager::createEnemy(true, random + 1, random1, 0, GAME_WIDTH / 2 - 30, 30, 1000, 30, 50, M_PI / 2, 0, -10);
                }
                else if (timer > 30 || EnemyManager::enemies.size() == 0)
                {
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
}
