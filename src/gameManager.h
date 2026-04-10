#pragma once
#include "constants.h"
#include "board.h"
class Game
{
	int gameMode;
	sf::RenderWindow* windowPointer;
	Board gameBoard;
    bool W;
    bool A;
    bool S;
    bool D;
    bool selected;
    bool unselected;
    int selection;
    bool pressedPause;
    bool releasedPause;
    bool stop;

    sf::Clock clock;
    sf::Time time;
public:
    Game(sf::RenderWindow* window);
    void processBasicEvents(sf::Event* event);
    void processInGameEvents(sf::Event* event);
    void drawGame();
    void mainGameProcess(double dt, sf::Event* event);
    void mainMenuProcess(double dt, sf::Event* event);
    void toNewStage();
    void processPauseScreen();
    void processGameOver();
    void process();
};