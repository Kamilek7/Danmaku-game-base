#include "sound.h"

std::map <std::string, sf::SoundBuffer> SoundManager::soundCache = {};
sf::Sound SoundManager::voices[20] = {};
sf::SoundBuffer SoundManager::buffers[20] = {};
void SoundManager::playSound(std::string filename)
{
    for (int i = 0; i < 20; i++)
        if (voices[i].getStatus() != sf::SoundSource::Status::Playing)
        {
            auto& temp = soundCache[filename];
            if (temp.getSampleCount() == 0)
            {
                temp.loadFromFile("resources/sfx/" + filename);
                soundCache[filename] = temp;
            }
            voices[i].setBuffer(temp);
            voices[i].play();
            break;
        }
}