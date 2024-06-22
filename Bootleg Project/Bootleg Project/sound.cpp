#include "sound.h"

sf::Sound SoundManager::voices[10] = {};
sf::SoundBuffer SoundManager::buffers[10] = {};
void SoundManager::playSound(std::string filename)
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