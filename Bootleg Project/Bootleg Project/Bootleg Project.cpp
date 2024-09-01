#include "board.h"
#include "gameManager.h"
#include <ctime>


int main()
{

    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(APP_WIDTH, APP_HEIGHT), "Bootleg Project");
    Game mainGameManager(&window);

    while (window.isOpen())
    {
        mainGameManager.process();
        
    }
    return 0;
}