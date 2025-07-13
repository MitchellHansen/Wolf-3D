#pragma once
#include "Camera.h"
#include <SFML/Graphics.hpp>
#include <cmath>

inline bool WorldToScreen(const sf::Vector2f& pos,
                          const Camera& cam,
                          float fovDeg,
                          sf::Vector2u screen,
                          sf::Vector2f& outPos,
                          float& outScale)
{
    sf::Vector3f camPos = cam.getPosition();
    float yaw = cam.getDirectionPolar().y;
    float dx = pos.x - camPos.x;
    float dy = pos.y - camPos.y;
    float angle = std::atan2(dy, dx) - yaw;
    while (angle < -PI_F) angle += 2 * PI_F;
    while (angle > PI_F) angle -= 2 * PI_F;
    float fovRad = DegreesToRadians(fovDeg);
    float half = fovRad / 2.f;
    if (std::abs(angle) > half)
        return false;
    float normX = (angle + half) / fovRad;
    outPos.x = normX * static_cast<float>(screen.x);
    outPos.y = screen.y / 2.f;
    float dist = std::sqrt(dx * dx + dy * dy);
    if (dist < 0.01f) dist = 0.01f;
    outScale = 1.f / dist;
    return true;
}
