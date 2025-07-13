#pragma once
#include <util.hpp>
#include <SFML/System.hpp>

class Camera {


public:

    Camera();
    ~Camera();

    void update(double delta_time, class Map* map = nullptr);

    void giveImpulse(sf::Vector3f direction, float value);
    void setPosition(sf::Vector3f position);
    sf::Vector3f getPosition () const;

    void setDirection (sf::Vector2f direction);
    void moveDirection (sf::Vector2f direction);
    sf::Vector2f getDirectionPolar() const;
    sf::Vector3f getDirectionCartesian() const;

private:

    float friction = 0.2;

    sf::Vector3f position;
    sf::Vector2f direction;
    sf::Vector3f movement_delta;

};