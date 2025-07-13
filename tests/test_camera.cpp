#include "Camera.h"
#include <cassert>

int main() {
    Camera cam;
    cam.setPosition(sf::Vector3f(0.f, 0.f, 0.f));
    cam.setDirection(sf::Vector2f(0.f, 0.f)); // yaw = 0 -> facing +X

    // give impulse forward in local coordinates
    cam.giveImpulse(sf::Vector3f(0.f, 1.f, 0.f), 1.f);
    cam.update(0.01, nullptr);
    sf::Vector3f pos = cam.getPosition();
    // should have moved mostly along +X
    assert(pos.x > 0.f && std::abs(pos.y) < 0.001f);

    // second test with new camera instance to avoid residual momentum
    Camera cam2;
    cam2.setPosition(sf::Vector3f(0.f, 0.f, 0.f));
    cam2.setDirection(sf::Vector2f(0.f, PI_F / 2.f)); // yaw = 90deg
    cam2.giveImpulse(sf::Vector3f(0.f, 1.f, 0.f), 1.f);
    cam2.update(0.01, nullptr);
    pos = cam2.getPosition();
    assert(pos.y > 0.f && std::abs(pos.x) < 0.001f);

    // strafing right at yaw=0 should increase Y
    Camera cam3;
    cam3.setPosition(sf::Vector3f(0.f, 0.f, 0.f));
    cam3.setDirection(sf::Vector2f(0.f, 0.f));
    cam3.giveImpulse(sf::Vector3f(1.f, 0.f, 0.f), 1.f);
    cam3.update(0.01, nullptr);
    pos = cam3.getPosition();
    assert(pos.y > 0.f && std::abs(pos.x) < 0.001f);

    // strafing right at yaw=90deg should decrease X
    Camera cam4;
    cam4.setPosition(sf::Vector3f(0.f, 0.f, 0.f));
    cam4.setDirection(sf::Vector2f(0.f, PI_F / 2.f));
    cam4.giveImpulse(sf::Vector3f(1.f, 0.f, 0.f), 1.f);
    cam4.update(0.01, nullptr);
    pos = cam4.getPosition();
    assert(pos.x < 0.f && std::abs(pos.y) < 0.001f);
    return 0;
}
