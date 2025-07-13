#pragma once
#include <SFML/Graphics.hpp>
#include "Animator.h"

class Enemy {
public:
    Enemy();
    bool load(const std::string &spriteSheetPath);
    void setPosition(const sf::Vector2f &pos);
    sf::Vector2f getPosition() const;
    void update(float dt);
    void draw(sf::RenderWindow &window);
    int getCurrentFrame() const { return animator.getFrame(); }

private:
    sf::Texture texture;
    sf::Sprite sprite;
    int frameWidth{64};
    int frameHeight{64};
    Animator animator{4, 0.2f};
};
