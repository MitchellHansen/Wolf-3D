#include "util.hpp"
#include <cassert>

int main() {
    sf::Vector2f pos(5.f, 10.f);
    sf::Vector3i dims(30, 50, 1);
    float scale = 800.f / static_cast<float>(std::max(dims.x, dims.y));
    sf::Vector2f screen(pos.x * scale, pos.y * scale);
    assert(screen.x >= 0.f && screen.x <= 800.f);
    assert(screen.y >= 0.f && screen.y <= 800.f);
    return 0;
}
