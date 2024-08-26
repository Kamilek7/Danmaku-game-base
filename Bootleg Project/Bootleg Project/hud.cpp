#include "hud.h"

HUDManager::HUDManager()
{
    font.loadFromFile("resources\\fonts\\exotic.otf");
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
    Power.setFont(font);
    Score.setFont(font);
    Lives.setFont(font);
    Power.setOutlineThickness(2);
    Score.setOutlineThickness(2);
    Lives.setOutlineThickness(2);
    for (int i = 0; i < Options.size(); i++)
    {
        Options[i].setFont(font);
        Options[i].setOutlineThickness(2);
    }
}

void HUDManager::updateHUD()
{
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
HUDManager HUDTextures = HUDManager();