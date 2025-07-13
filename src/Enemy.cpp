#include "Enemy.h"

Enemy::Enemy() {}

bool Enemy::load(const std::string &spriteSheetPath) {
    if (!texture.loadFromFile(spriteSheetPath))
        return false;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, frameHeight, frameWidth, frameHeight));
    return true;
}

void Enemy::setPosition(const sf::Vector2f &pos) {
    sprite.setPosition(pos);
}

sf::Vector2f Enemy::getPosition() const {
    return sprite.getPosition();
}

void Enemy::update(float dt) {
    animator.update(dt);
    sprite.setTextureRect(sf::IntRect(animator.getFrame() * frameWidth,
                                      frameHeight, frameWidth, frameHeight));
}

void Enemy::draw(sf::RenderWindow &window) {
    window.draw(sprite);
}
