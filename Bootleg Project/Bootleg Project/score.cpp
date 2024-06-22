#include "score.h"

unsigned long int ScoreManager::internalScore = 0;
unsigned long int ScoreManager::score = 0;
int ScoreManager::lives = 5;
int ScoreManager::power = 0;

void ScoreManager::reset()
{
    internalScore = 0;
    score = 0;
    lives = 0;
    power = 0;
}

void ScoreManager::changeScores(int value)
{
    internalScore += value;
    score += value;
    if (internalScore >= 100000)
    {
        internalScore -= 100000;
        lives += 1;
        SoundManager::playSound("se_extend.wav");
    }
}

void ScoreManager::increasePower(int value)
{
    if (power < 500)
        power+=value;
    if (!(power % 100))
        SoundManager::playSound("se_powerup.wav");
}


void ScoreManager::cutPower()
{
    power /= 2;
}

void ScoreManager::changeLives(int value)
{
    lives += value;
}

long unsigned int ScoreManager::getScore()
{
    return score;
}

long unsigned int ScoreManager::getInternalScore()
{
    return internalScore;
}

int ScoreManager::getLives()
{
    return lives;
}

int ScoreManager::getPower()
{
    return power;
}
