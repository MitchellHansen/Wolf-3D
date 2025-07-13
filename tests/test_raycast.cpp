#include "Map.h"
#include <cassert>
#include <cmath>
#include "util.hpp"

static bool castRay(Map& map, sf::Vector3f pos, float yaw)
{
    sf::Vector3f dir(std::cos(yaw), std::sin(yaw), 0.f);
    int mapX = static_cast<int>(pos.x);
    int mapY = static_cast<int>(pos.y);

    float deltaDistX = dir.x == 0 ? 1e30f : std::abs(1.f / dir.x);
    float deltaDistY = dir.y == 0 ? 1e30f : std::abs(1.f / dir.y);

    int stepX = (dir.x < 0) ? -1 : 1;
    int stepY = (dir.y < 0) ? -1 : 1;

    float sideDistX = (dir.x < 0) ? (pos.x - mapX) * deltaDistX : (mapX + 1.f - pos.x) * deltaDistX;
    float sideDistY = (dir.y < 0) ? (pos.y - mapY) * deltaDistY : (mapY + 1.f - pos.y) * deltaDistY;

    int hit = 0;
    int mapDimX = map.getDimensions().x;
    int mapDimY = map.getDimensions().y;

    while (!hit)
    {
        if (sideDistX < sideDistY)
        {
            sideDistX += deltaDistX;
            mapX += stepX;
        }
        else
        {
            sideDistY += deltaDistY;
            mapY += stepY;
        }

        if (mapX < 0 || mapY < 0 || mapX >= mapDimX || mapY >= mapDimY)
            return false;

        if (map.getGrid(sf::Vector3i(mapX, mapY, 0)) > 0)
            return true;
    }
    return false;
}

int main() {
    Map map;
    map.Init(sf::Vector3i(3,3,1));
    map.setGrid(sf::Vector3i(2,1,0), 1); // place wall east of center

    sf::Vector3f pos(1.5f,1.5f,0.f);

    // facing east should hit wall
    assert(castRay(map, pos, 0.f));

    // facing north should miss
    assert(!castRay(map, pos, PI_F/2.f));

    return 0;
}
