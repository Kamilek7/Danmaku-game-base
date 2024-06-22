#pragma once
#include "sound.h"
class ScoreManager
{
	static long unsigned int internalScore;
	static long unsigned int score;
	static int lives;
	static int power;
public:
	static void reset();
	static void changeScores(int value);
	static void increasePower(int value);
	static void cutPower();
	static void changeLives(int value);
	static long unsigned int getScore();
	static long unsigned int getInternalScore();
	static int getLives();
	static int getPower();
};