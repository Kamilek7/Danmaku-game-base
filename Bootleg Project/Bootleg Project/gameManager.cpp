#include "gameManager.h"
Game::Game(sf::RenderWindow* window)
{
    gameMode= 0;
    windowPointer = window;
    W = false;
    A = false;
    S = false;
    D = false;
    selected = false;
    unselected = true;
    selection = 0;
    pressedPause = false;
    releasedPause = false;
    stop = false;
}

void Game::mainGameProcess(double dt, sf::Event* event)
{
    gameBoard.gameProcess(dt, gameMode);
    while (windowPointer->pollEvent(*event))
    {
        if (event->type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            windowPointer->close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            pressedPause = true;
        if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) && pressedPause)
            releasedPause = true;
        if (sf::Event::KeyPressed)
        {
            if (event->key.code == sf::Keyboard::W)
                W = true;
            if (event->key.code == sf::Keyboard::S)
                S = true;
            if (event->key.code == sf::Keyboard::A)
                A = true;
            if (event->key.code == sf::Keyboard::D)
                D = true;
            if (event->key.code == sf::Keyboard::Space)
            {
                if (!gameBoard.checkPause() && !gameBoard.isEndOfStage())
                    gameBoard.player->shooting = true;
                else
                {
                    selected = true;
                    unselected = false;
                }
            }
        }
        if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
            W = false;
        if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
            S = false;
        if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
            A = false;
        if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
            D = false;
        if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)))
        {
            if (!gameBoard.checkPause() && !gameBoard.isEndOfStage())
                gameBoard.player->shooting = false;
            else
                unselected = true;
        }
    }
    if (releasedPause && pressedPause)
    {
        pressedPause = false;
        releasedPause = false;
        stop = false;
    }
    if (pressedPause && !stop)
    {
        gameBoard.pauseUnpause();
        if (gameBoard.checkPause())
            SoundManager::playSound("se_pause.wav");
        else
            SoundManager::playSound("se_cancel00.wav");
        stop = true;
    }
    gameBoard.player->moveX(A, D);
    gameBoard.player->moveY(W, S);
    windowPointer->draw(HUDTextures.GAMEBOARD);
    for (int i = 0; i < PointsManager::points.size(); i++)
    {
        if (!(PointsManager::points[i]->isDestroyed()))
            windowPointer->draw(PointsManager::points[i]->getSprite());
    }
    if (!(gameBoard.player->isDestroyed()))
        windowPointer->draw(gameBoard.player->getSprite());
    for (int i = 0; i < EnemyManager::enemies.size(); i++)
    {
        if (!(EnemyManager::enemies[i]->isDestroyed()))
            windowPointer->draw(EnemyManager::enemies[i]->getSprite());
    }
    for (int i = 0; i < DanmakuManager::bullets.size(); i++)
    {
        if (!(DanmakuManager::bullets[i]->isDestroyed()))
            windowPointer->draw(DanmakuManager::bullets[i]->getSprite());
    }
    windowPointer->draw(HUDTextures.HUD);
    windowPointer->draw(HUDTextures.Score);
    windowPointer->draw(HUDTextures.Power);
    windowPointer->draw(HUDTextures.Lives);
}

void Game::process()
{
    sf::Event event;
    windowPointer->clear();
    time = clock.restart();
    float dt = time.asSeconds();
    if (gameMode != 0)
    {
        // Actual Game
        mainGameProcess(dt, &event);

    }
    else
    {
        // Main Menu

        while (windowPointer->pollEvent(event))
        {
            if (event.type == sf::Event::Closed || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
                windowPointer->close();
            if (sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::W)
                    W = true;
                if (event.key.code == sf::Keyboard::S)
                    S = true;
                if (event.key.code == sf::Keyboard::A)
                    A = true;
                if (event.key.code == sf::Keyboard::D)
                    D = true;
            }
            if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
                W = false;
            if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
                S = false;
            if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
                A = false;
            if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
                D = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                SoundManager::playSound("se_ok00.wav");
                gameMode = 1 + selection;
            }
        }
        if (A || S)
        {
            selection--;
            A = false;
            S = false;
            SoundManager::playSound("se_select00.wav");
        }
        else if (D || W)
        {
            selection++;
            D = false;
            W = false;
            SoundManager::playSound("se_select00.wav");
        }
        if (selection < 0)
            selection = HUDTextures.Options.size() - 1;
        else if (selection >= HUDTextures.Options.size())
            selection = 0;
        windowPointer->draw(HUDTextures.MainMenu);
        for (int i = 0; i < HUDTextures.Options.size(); i++)
        {
            if (i == selection)
                HUDTextures.Options[i].setOutlineColor(sf::Color::Red);
            else
                HUDTextures.Options[i].setOutlineColor(sf::Color::Black);
            windowPointer->draw(HUDTextures.Options[i]);
        }
    }
    if (gameBoard.isEndOfStage())
    {
        // New Stage

        sf::RectangleShape effect;
        effect.setSize(sf::Vector2f(APP_WIDTH, APP_HEIGHT));
        effect.setFillColor(sf::Color(0, 0, 0, 200));
        windowPointer->draw(effect);
        sf::Text opt1;
        opt1.setString("Continue to next stage");
        opt1.setPosition(APP_WIDTH / 4.25, APP_HEIGHT / 1.3 + 10);
        opt1.setOutlineThickness(2);
        opt1.setFont(HUDTextures.font);
        opt1.setFillColor(sf::Color::Yellow);
        windowPointer->draw(opt1);
        if (selected && unselected && !gameBoard.checkPause())
        {
            SoundManager::playSound("se_ok00.wav");
            gameBoard.nextStage();
            selected = false;
        }
    }
    if (gameBoard.checkPause() && ScoreManager::getLives() > 0)
    {
        // Game Pause

        static int choice = 0;
        sf::RectangleShape effect;
        effect.setSize(sf::Vector2f(APP_WIDTH, APP_HEIGHT));
        effect.setFillColor(sf::Color(0, 0, 0, 150));
        windowPointer->draw(effect);
        sf::Text pause;
        pause.setString("- Game Paused -");
        pause.setOutlineThickness(2);
        pause.setFont(HUDTextures.font);
        pause.setPosition(APP_WIDTH / 3.85, APP_HEIGHT / 3 - 60);
        pause.setCharacterSize(34);
        windowPointer->draw(pause);
        std::vector <sf::Text> opcje;
        sf::Text opt1;
        opt1.setString("Return to game");
        opt1.setPosition(APP_WIDTH / 3.65, APP_HEIGHT / 3 + 10);
        sf::Text opt2;
        opt2.setString("Restart game");
        opt2.setPosition(APP_WIDTH / 3.5, APP_HEIGHT / 3 + 60);
        sf::Text opt3;
        opt3.setString("Return to main menu");
        opt3.setPosition(APP_WIDTH / 3.98, APP_HEIGHT / 3 + 110);
        opcje.push_back(opt1);
        opcje.push_back(opt2);
        opcje.push_back(opt3);
        if (selected && unselected)
        {
            SoundManager::playSound("se_ok00.wav");
            if (choice == 0)
                gameBoard.pauseUnpause();
            else if (choice == 1)
                gameBoard.restart();
            else if (choice == 2)
            {
                gameBoard.restart();
                gameMode = 0;
            }
            selected = false;
        }
        if (D || S)
        {
            choice++;
            D = false;
            S = false;
            SoundManager::playSound("se_select00.wav");
        }
        else if (A || W)
        {
            choice--;
            A = false;
            W = false;
            SoundManager::playSound("se_select00.wav");
        }
        if (choice < 0)
            choice = 2;
        else if (choice >= 3)
            choice = 0;
        for (int i = 0; i < 3; i++)
        {
            opcje[i].setOutlineThickness(2);
            opcje[i].setFont(HUDTextures.font);
            if (choice == i)
                opcje[i].setFillColor(sf::Color::Yellow);
            windowPointer->draw(opcje[i]);
        }
    }
    else if (ScoreManager::getLives() == 0)
    {
        // Game Over

        static int choice = 0;
        sf::RectangleShape effect;
        effect.setSize(sf::Vector2f(APP_WIDTH, APP_HEIGHT));
        effect.setFillColor(sf::Color(0, 0, 0, 150));
        windowPointer->draw(effect);
        sf::Text pause;
        pause.setString("- Game Over -");
        pause.setOutlineThickness(2);
        pause.setFont(HUDTextures.font);
        pause.setPosition(APP_WIDTH / 3.67, APP_HEIGHT / 3 - 60);
        pause.setCharacterSize(34);
        windowPointer->draw(pause);
        std::vector <sf::Text> opcje;
        sf::Text opt2;
        opt2.setString("Restart game");
        opt2.setPosition(APP_WIDTH / 3.5, APP_HEIGHT / 3 + 10);
        sf::Text opt3;
        opt3.setString("Return to main menu");
        opt3.setPosition(APP_WIDTH / 3.98, APP_HEIGHT / 3 + 60);
        opcje.push_back(opt2);
        opcje.push_back(opt3);
        if (selected && unselected)
        {
            SoundManager::playSound("se_ok00.wav");
            if (choice == 0)
                gameBoard.restart();
            else if (choice == 1)
            {
                gameBoard.restart();
                gameMode = 0;
            }
            selected = false;
        }
        if (D || S)
        {
            choice++;
            D = false;
            S = false;
            SoundManager::playSound("se_select00.wav");
        }
        else if (A || W)
        {
            choice--;
            A = false;
            W = false;
            SoundManager::playSound("se_select00.wav");
        }
        if (choice < 0)
            choice = 1;
        else if (choice >= 2)
            choice = 0;
        for (int i = 0; i < 2; i++)
        {
            opcje[i].setOutlineThickness(2);
            opcje[i].setFont(HUDTextures.font);
            if (choice == i)
                opcje[i].setFillColor(sf::Color::Yellow);
            windowPointer->draw(opcje[i]);
        }
    }
    windowPointer->display();
}
