#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include <cassert>

int main(){
    Enemy e;
    assert(e.load("assets/elite-guard.png"));
    sf::RenderTexture tex;
    tex.create(128,128);
    e.setScreenPosition(sf::Vector2f(64.f,64.f));
    tex.clear(sf::Color::Black);
    e.drawSprite(tex);
    tex.display();
    sf::Image img = tex.getTexture().copyToImage();
    unsigned long sum = 0;
    for(unsigned y=0;y<img.getSize().y;++y)
        for(unsigned x=0;x<img.getSize().x;++x)
            sum += img.getPixel(x,y).a;
    assert(sum>0);
    return 0;
}
