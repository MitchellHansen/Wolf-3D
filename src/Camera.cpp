#include <Camera.h>

Camera::Camera(){

}
Camera::~Camera(){

}

void Camera::giveImpulse(sf::Vector3f direction, float value){
    movement_delta = direction * value;
}

void Camera::setPosition(sf::Vector3f position){
    this->position = position;
}

sf::Vector3f Camera::getPosition () const {
    return position;
}

void Camera::update(double delta_time){
    movement_delta *= (1.0f - friction);
    position += (movement_delta * (float)delta_time * 1000.0f);
}

void Camera::setDirection (sf::Vector2f direction){
    this->direction = direction;
}

void Camera::moveDirection (sf::Vector2f direction){
    this->direction += direction;
}

sf::Vector2f Camera::getDirectionPolar(){
    return direction;
}

sf::Vector3f Camera::getDirectionCartesian(){
    return SphereToCart(direction);
}