#include <Camera.h>
#include <Map.h>

Camera::Camera(){

}
Camera::~Camera(){

}

void Camera::giveImpulse(sf::Vector3f direction_local, float value){
    // Convert local direction to world space using the camera yaw so that
    // movement is relative to where the camera is facing. The direction vector
    // provided here follows a conventional scheme where X is "strafe", Y is
    // "forward" and Z is up.

    float yaw = this->direction.y;

    // Basis vectors for the camera in world coordinates (ignoring pitch for
    // movement).
    sf::Vector3f forward(std::cos(yaw), std::sin(yaw), 0.f);
    // Right handed system: right = forward x up
    sf::Vector3f right(forward.y, -forward.x, 0.f);
    sf::Vector3f up(0.f, 0.f, 1.f);

    sf::Vector3f world_dir = right * direction_local.x +
                             forward * direction_local.y +
                             up * direction_local.z;

    // Accumulate movement so multiple keys can be pressed simultaneously.
    movement_delta += world_dir * value;
}

void Camera::setPosition(sf::Vector3f position){
    this->position = position;
}

sf::Vector3f Camera::getPosition () const {
    return position;
}

void Camera::update(double delta_time, Map* map){
    movement_delta *= (1.0f - friction);
    sf::Vector3f new_pos = position + (movement_delta * static_cast<float>(delta_time) * 1000.0f);

    if (map) {
        sf::Vector3f test = position;

        // X axis
        test.x = new_pos.x;
        if (!map->isWall(test))
            position.x = test.x;

        // Y axis
        test = position;
        test.y = new_pos.y;
        if (!map->isWall(test))
            position.y = test.y;

        // Z axis
        test = position;
        test.z = new_pos.z;
        if (!map->isWall(test))
            position.z = test.z;
    } else {
        position = new_pos;
    }
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