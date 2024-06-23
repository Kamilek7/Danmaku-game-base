#include "pointsOnBoard.h"

std::vector<Points*> PointsManager::points = {};

void PointsManager::createRandomScorePoints(cords pos)
{
	char _type;
	int _value;
	if (rand() % 2)
	{
		_type = 'p';
		_value = 1;
	}
	else
	{
		_type = 's';
		_value = 1000;
	}
	Points* temp = new Points(pos.x + ((rand() % 20) * 2 * pow(-1, rand() % 2)), pos.y + ((rand() % 20) * 2 * pow(-1, rand() % 2)), _type, _value);
	points.insert(points.end(), temp);
}

void PointsManager::pointsUpdate(float dt)
{
    for (int i = 0; i < points.size(); i++)
        points[i]->process(dt);
}
