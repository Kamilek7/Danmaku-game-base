#include "animatedSprite.h"
#include "animation.h"
#include "points.h"
#include "bullet.h"
#include "player.h"
#include "board.h"

#include <string>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <ctime>

extern const float APP_WIDTH;
extern const float APP_HEIGHT;
extern const float GAME_WIDTH;
extern const float GAME_HEIGHT;
extern const float H_OFFSET;
extern const float W_OFFSET;
int main()
{
    sf::Font font;
    font.loadFromFile("resources\\fonts\\exotic.otf");
    srand(time(NULL));
    int gameMode = 0;
    Board game;
    game.Power.setFont(font);
    game.Score.setFont(font);
    game.Lives.setFont(font);
    game.Power.setOutlineThickness(2);
    game.Score.setOutlineThickness(2);
    game.Lives.setOutlineThickness(2);
    for (int i = 0; i < game.Options.size(); i++)
    {
        game.Options[i].setFont(font);
        game.Options[i].setOutlineThickness(2);
    }
    sf::RenderWindow window(sf::VideoMode(APP_WIDTH, APP_HEIGHT), "Bootleg Project");
    sf::Clock clock;
    sf::Time time;
    bool W = false;
    bool A = false;
    bool S = false;
    bool D = false;
    bool selected = false;
    bool unselected = true;
    int selection = 0;
    bool pressedPause = false;
    bool releasedPause = false;
    bool stop = false;
    while (window.isOpen())
    {
        sf::Event event;
        window.clear();
        time = clock.restart();
        float dt = time.asSeconds();
        if (gameMode != 0)
        {
            game.gameProcess(dt, gameMode);
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    window.close();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                    pressedPause = true;
                if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) && pressedPause)
                    releasedPause = true;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
                    game.a();
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
                    if (event.key.code == sf::Keyboard::Space)
                    {
                        if (!game.checkPause() && !game.isEndOfStage())
                            game.player->shooting = true;
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
                    if (!game.checkPause() && !game.isEndOfStage())
                        game.player->shooting = false;
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
                game.pauseUnpause();
                if (game.checkPause())
                    SoundManager::playSound("se_pause.wav");
                else
                    SoundManager::playSound("se_cancel00.wav");
                stop = true;
            }
            game.player->moveX(A, D);
            game.player->moveY(W, S);
            window.draw(game.GAMEBOARD);
            for (int i = 0; i < PointsManager::points.size(); i++)
            {
                if (!(PointsManager::points[i]->isDestroyed()))
                    window.draw(PointsManager::points[i]->getSprite());
            }
                if (!(game.player->isDestroyed()))
                    window.draw(game.player->getSprite());
            for (int i = 0; i < EnemyManager::enemies.size(); i++)
            {
                if (!(EnemyManager::enemies[i]->isDestroyed()))
                    window.draw(EnemyManager::enemies[i]->getSprite());
            }
            for (int i = 0; i < DanmakuManager::bullets.size(); i++)
            {
                if (!(DanmakuManager::bullets[i]->isDestroyed()))
                    window.draw(DanmakuManager::bullets[i]->getSprite());
            }
            window.draw(game.HUD);
            window.draw(game.Score);
            window.draw(game.Power);
            window.draw(game.Lives);
        }
        else
        {
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
                    window.close();
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
                selection = game.Options.size() - 1;
            else if (selection >= game.Options.size())
                selection = 0;
            window.draw(game.MainMenu);
            for (int i = 0; i < game.Options.size(); i++)
            {
                if (i == selection)
                    game.Options[i].setOutlineColor(sf::Color::Red);
                else
                    game.Options[i].setOutlineColor(sf::Color::Black);
                window.draw(game.Options[i]);
            }
        }
        if (game.isEndOfStage())
        {
            sf::RectangleShape effect;
            effect.setSize(sf::Vector2f(APP_WIDTH, APP_HEIGHT));
            effect.setFillColor(sf::Color(0, 0, 0, 200));
            window.draw(effect);
            sf::Text opt1;
            opt1.setString("Continue to next stage");
            opt1.setPosition(APP_WIDTH / 4.25, APP_HEIGHT / 1.3 + 10);
            opt1.setOutlineThickness(2);
            opt1.setFont(font);
            opt1.setFillColor(sf::Color::Yellow);
            window.draw(opt1);
            if (selected && unselected && !game.checkPause())
            {
                SoundManager::playSound("se_ok00.wav");
                game.nextStage();
                selected = false;
            }
        }
        if (game.checkPause() && ScoreManager::getLives()>0)
        {
            static int choice = 0;
            sf::RectangleShape effect;
            effect.setSize(sf::Vector2f(APP_WIDTH, APP_HEIGHT));
            effect.setFillColor(sf::Color(0, 0, 0, 150));
            window.draw(effect);
            sf::Text pause;
            pause.setString("- Game Paused -");
            pause.setOutlineThickness(2);
            pause.setFont(font);
            pause.setPosition(APP_WIDTH/3.85,APP_HEIGHT/3-60);
            pause.setCharacterSize(34);
            window.draw(pause);
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
                    game.pauseUnpause();
                else if (choice == 1)
                    game.restart();
                else if (choice == 2)
                {
                    game.restart();
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
                opcje[i].setFont(font);
                if (choice == i)
                    opcje[i].setFillColor(sf::Color::Yellow);
                window.draw(opcje[i]);
            }
        }
        else if (ScoreManager::getLives() == 0)
        {
            static int choice = 0;
            sf::RectangleShape effect;
            effect.setSize(sf::Vector2f(APP_WIDTH, APP_HEIGHT));
            effect.setFillColor(sf::Color(0, 0, 0, 150));
            window.draw(effect);
            sf::Text pause;
            pause.setString("- Game Over -");
            pause.setOutlineThickness(2);
            pause.setFont(font);
            pause.setPosition(APP_WIDTH / 3.67, APP_HEIGHT / 3 - 60);
            pause.setCharacterSize(34);
            window.draw(pause);
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
                    game.restart();
                else if (choice == 1)
                {
                    game.restart();
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
                opcje[i].setFont(font);
                if (choice == i)
                    opcje[i].setFillColor(sf::Color::Yellow);
                window.draw(opcje[i]);
            }
        }
        window.display();
    }
    return 0;
}