#include "board.h"
#include "gameManager.h"
#include <ctime>


int main()
{

    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(APP_WIDTH, APP_HEIGHT), "Bootleg Project");
    Game mainGameManager(&window);

    //int gameMode = 0;
    //sf::Clock clock;
    //sf::Time time;
    //bool W = false;
    //bool A = false;
    //bool S = false;
    //bool D = false;
    //bool selected = false;
    //bool unselected = true;
    //int selection = 0;
    //bool pressedPause = false;
    //bool releasedPause = false;
    //bool stop = false;
    while (window.isOpen())
    {
        mainGameManager.process();
        
    }
    return 0;
}