#include "Enemy.h"

Enemy::Enemy() {}

bool Enemy::load(const std::string &spriteSheetPath) {
    if (!texture.loadFromFile(spriteSheetPath))
        return false;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, frameHeight, frameWidth, frameHeight));
    sprite.setOrigin(frameWidth / 2.f, frameHeight);
    return true;
}

void Enemy::setPosition(const sf::Vector2f &pos) {
    position = pos;
}

sf::Vector2f Enemy::getPosition() const {
    return position;
}

void Enemy::update(float dt) {
    animator.update(dt);
    sprite.setTextureRect(sf::IntRect(animator.getFrame() * frameWidth,
                                      frameHeight, frameWidth, frameHeight));
    position += velocity * dt;
}

void Enemy::draw(sf::RenderTarget &target, const Camera &camera, float fovDeg) {
    sf::Vector2f screenPos;
    float scale;
    sf::Vector2u size = target.getSize();
    if (!WorldToScreen(position, camera, fovDeg, size, screenPos, scale))
        return;
    float baseSize = 150.f; // constant for on-screen scaling
    float pixelSize = baseSize * scale;
    sprite.setPosition(screenPos);
    sprite.setScale(pixelSize / frameWidth, pixelSize / frameHeight);
    sprite.setOrigin(frameWidth / 2.f, frameHeight);
    target.draw(sprite);
}
