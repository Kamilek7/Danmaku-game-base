#pragma once
#include "points.h"

class PointsManager
{
public:
	static std::vector <Points*> points;

	static void createRandomScorePoints(cords pos);
	static void pointsUpdate(float dt);
	static void reset();
};