#pragma once
#include <SFML/Graphics.hpp>
#include "Animator.h"
#include "Camera.h"
#include "Projection.h"

class Enemy {
public:
    Enemy();
    bool load(const std::string &spriteSheetPath);
    void setPosition(const sf::Vector2f &pos);
    sf::Vector2f getPosition() const;
    void setVelocity(const sf::Vector2f &v) { velocity = v; }
    void update(float dt);
    void draw(sf::RenderTarget &target, const Camera &camera, float fovDeg);
    int getCurrentFrame() const { return animator.getFrame(); }

private:
    sf::Texture texture;
    sf::Sprite sprite;
    int frameWidth{64};
    int frameHeight{64};
    Animator animator{4, 0.2f};
    sf::Vector2f velocity{0.f, 0.f};
    sf::Vector2f position{0.f, 0.f};
};
