#include "Enemy.h"
#include "Camera.h"
#include "Map.h"
#include "Raycaster.h"
#include <SFML/Graphics.hpp>
#include <cassert>

int main() {
    Enemy e;
    assert(e.load("assets/elite-guard.png"));
    e.setPosition(sf::Vector2f(2.f, 0.f));
    std::shared_ptr<Camera> cam(new Camera);
    cam->setPosition(sf::Vector3f(0.f, 0.f, 0.f));
    cam->setDirection(sf::Vector2f(0.f, 0.f));

    Map map;
    map.Init(sf::Vector3i(10,10,1));

    Raycaster ray(&map, cam);
    ray.CreateViewport(sf::Vector2i(200,200), sf::Vector2f(60.f,60.f));
    ray.LoadTextures();
    ray.Cast();

    sf::RenderWindow window(sf::VideoMode(200,200), "test", sf::Style::Default);
    window.clear(sf::Color::Black);
    ray.Draw(&window);
    e.draw(window, *cam, 60.f);
    window.display();

    sf::Texture capture;
    capture.create(200,200);
    capture.update(window);
    sf::Image img = capture.copyToImage();
    sf::Vector2u size = img.getSize();
    unsigned long sum = 0;
    for(unsigned y=0; y<size.y; ++y)
        for(unsigned x=0; x<size.x; ++x)
            sum += img.getPixel(x,y).a;
    assert(sum > 0);
    return 0;
}
