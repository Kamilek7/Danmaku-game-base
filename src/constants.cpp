#include "constants.h"

std::vector<cords> adjacentTiles(int i, int j)
{
    int num = j * 6 + i;
    std::vector<cords> cors;
    cords temp;
    int i1 = num - 6;
    int i2 = num - 1;
    int i3 = num + 1;
    int i4 = num + 6;
    temp.x = i;
    temp.y = j;
    cors.push_back(temp);
    if (i1 >= 0)
    {
        temp.x = i1 % 6;
        temp.y = int(i1 / 6);
        cors.push_back(temp);
    }
    if ((i2 % 6) != 5 && i2 >= 0)
    {
        temp.x = i2 % 6;
        temp.y = int(i2 / 6);
        cors.push_back(temp);
    }
    if ((i3 % 6) != 0)
    {
        temp.x = i3 % 6;
        temp.y = int(i3 / 6);
        cors.push_back(temp);
    }
    if (i4 < 42)
    {
        temp.x = i4 % 6;
        temp.y = int(i4 / 6);
        cors.push_back(temp);
    }
    return cors;
}
