#pragma once
#include "constants.h"
class HUDManager
{
public:
    sf::Font font;
    sf::Texture GameBackground;
    sf::Texture HUDt;
    sf::Texture MainMenuT;
    sf::Sprite HUD;
    sf::Sprite GAMEBOARD;
    sf::Sprite MainMenu;
    sf::Text Score;
    sf::Text Power;
    sf::Text Lives;
    std::vector <sf::Text> Options;
    HUDManager();
    void updateHUD();
};
