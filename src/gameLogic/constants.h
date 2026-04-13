#pragma once

#define _USE_MATH_DEFINES
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include "score.h"
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
const float APP_WIDTH = 1200;
const float APP_HEIGHT = 900;
const float GAME_WIDTH = 720;
const float GAME_HEIGHT = 840;
const float H_OFFSET = 30;
const float W_OFFSET = 60;
const float SPEED = 300;
const int ENEMY_TYPES = 5;
const int MIDBOSS_TYPES = 1;
const int SPELLCARDS = 3;

struct cords
{
    float x = 0;
    float y = 0;
};
std::vector<cords> adjacentTiles(int i, int j);
