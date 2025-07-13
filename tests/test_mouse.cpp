#include "Camera.h"
#include "Map.h"
#include <cassert>
#include <cmath>

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

    int mapDimX = map.getDimensions().x;
    int mapDimY = map.getDimensions().y;

    while (true)
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
}

int main() {
    Map map;
    map.Init(sf::Vector3i(3,3,1));
    map.setGrid(sf::Vector3i(2,1,0), 1); // wall east of center

    sf::Vector3f start(1.5f,1.5f,0.f);

    // baseline - facing east hits wall
    Camera cam1;
    cam1.setPosition(start);
    cam1.setDirection(sf::Vector2f(0.f, 0.f));
    assert(castRay(map, cam1.getPosition(), cam1.getDirectionPolar().y));

    // mouse move left -> yaw +90 deg -> should miss east wall
    Camera cam2;
    cam2.setPosition(start);
    cam2.setDirection(sf::Vector2f(0.f, 0.f));
    cam2.moveDirection(sf::Vector2f(0.f, PI_F / 2.f));
    sf::Vector2f dir2 = cam2.getDirectionPolar();
    assert(std::abs(dir2.y - PI_F / 2.f) < 0.0001f);
    assert(!castRay(map, cam2.getPosition(), dir2.y));

    // mouse move right -> yaw -90 deg -> should miss east wall
    Camera cam3;
    cam3.setPosition(start);
    cam3.setDirection(sf::Vector2f(0.f, 0.f));
    cam3.moveDirection(sf::Vector2f(0.f, -PI_F / 2.f));
    sf::Vector2f dir3 = cam3.getDirectionPolar();
    assert(std::abs(dir3.y + PI_F / 2.f) < 0.0001f);
    assert(!castRay(map, cam3.getPosition(), dir3.y));

    // mouse move up -> pitch positive, yaw unchanged -> still hit east wall
    Camera cam4;
    cam4.setPosition(start);
    cam4.setDirection(sf::Vector2f(0.f, 0.f));
    cam4.moveDirection(sf::Vector2f(0.5f, 0.f));
    sf::Vector2f dir4 = cam4.getDirectionPolar();
    assert(dir4.x > 0.f && std::abs(dir4.y) < 0.0001f);
    assert(castRay(map, cam4.getPosition(), dir4.y));

    // mouse move down -> pitch negative, yaw unchanged -> still hit east wall
    Camera cam5;
    cam5.setPosition(start);
    cam5.setDirection(sf::Vector2f(0.f, 0.f));
    cam5.moveDirection(sf::Vector2f(-0.5f, 0.f));
    sf::Vector2f dir5 = cam5.getDirectionPolar();
    assert(dir5.x < 0.f && std::abs(dir5.y) < 0.0001f);
    assert(castRay(map, cam5.getPosition(), dir5.y));

    // After rotation, no further mouse movement should keep orientation stable
    Camera cam6;
    cam6.setPosition(start);
    cam6.setDirection(sf::Vector2f(0.f, 0.f));
    cam6.moveDirection(sf::Vector2f(0.f, PI_F / 4.f));
    sf::Vector2f before = cam6.getDirectionPolar();
    cam6.moveDirection(sf::Vector2f(0.f, 0.f));
    sf::Vector2f after = cam6.getDirectionPolar();
    assert(std::abs(after.y - before.y) < 1e-6f);

    return 0;
}
