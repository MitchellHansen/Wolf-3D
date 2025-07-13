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
    return 0;
}
