#include "Projection.h"
#include <cassert>
#include <cmath>

int main() {
    Camera cam;
    cam.setPosition(sf::Vector3f(0.f, 0.f, 0.f));
    cam.setDirection(sf::Vector2f(0.f, 0.f));

    sf::Vector2f screen;
    float scale;
    bool vis = WorldToScreen(sf::Vector2f(1.f, 0.f), cam, 60.f,
                             sf::Vector2u(800, 800), screen, scale);
    assert(vis);
    assert(std::abs(screen.x - 400.f) < 1.f);
    assert(scale > 0.f);
    return 0;
}
