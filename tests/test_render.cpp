#include "Enemy.h"
#include "Camera.h"
#include <SFML/Graphics.hpp>
#include <cassert>

int main() {
    Enemy e;
    assert(e.load("assets/elite-guard.png"));
    e.setPosition(sf::Vector2f(2.f, 0.f));
    Camera cam;
    cam.setPosition(sf::Vector3f(0.f, 0.f, 0.f));
    cam.setDirection(sf::Vector2f(0.f, 0.f));

    sf::RenderTexture tex;
    bool ok = tex.create(200, 200);
    assert(ok);
    tex.clear(sf::Color::Black);

    e.draw(tex, cam, 60.f);
    tex.display();

    sf::Image img = tex.getTexture().copyToImage();
    sf::Vector2u size = img.getSize();
    unsigned long sum = 0;
    for(unsigned y=0; y<size.y; ++y)
        for(unsigned x=0; x<size.x; ++x)
            sum += img.getPixel(x,y).a;
    assert(sum > 0);
    return 0;
}
