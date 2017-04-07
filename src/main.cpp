
#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>
#include <Camera.h>
#include <Map.h>
#include <Raycaster.h>
#include "util.hpp"

float elap_time() {
	static std::chrono::time_point<std::chrono::system_clock> start;
	static bool started = false;

	if (!started) {
		start = std::chrono::system_clock::now();
		started = true;
	}

	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_time = now - start;
	return static_cast<float>(elapsed_time.count());
}

int main() {
	
	std::mt19937 rng(time(NULL));
	std::uniform_int_distribution<int> rgen(100, 400);

	sf::RenderWindow window(sf::VideoMode(600, 600), "quick-sfml-template");
	//window.setFramerateLimit(60);


    std::shared_ptr<Camera> camera(new Camera);
    camera->setPosition(sf::Vector3f(3.1f, 3.1f, 3.1f));
    camera->setDirection(sf::Vector2f(1.0f, 0.0f));

    std::shared_ptr<Map> map(new Map);

    if (!map->loadGrid("grid.bin")){
        map->Init(sf::Vector3i(12, 12, 5));
    }

    Raycaster raycaster(map, camera);

    raycaster.CreateViewport(sf::Vector2i(400, 400), sf::Vector2f(80.0f, 80.0f));
    raycaster.LoadTextures();



	float physic_step = 0.166f;
	float physic_time = 0.0f;
	double frame_time = 0.0, elapsed_time = 0.0, delta_time = 0.0, accumulator_time = 0.0, current_time = 0.0;
	fps_counter fps;

	while (window.isOpen())
	{
		sf::Event event; // Handle input
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::MouseWheelScrolled) {
				
			}
			if (event.type == sf::Event::KeyPressed) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
                    camera->giveImpulse(sf::Vector3f(-0.001, 0, 0), 1.0);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                    camera->giveImpulse(sf::Vector3f(0, -0.001, 0), 1.0);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                    camera->giveImpulse(sf::Vector3f(0.001, 0, 0), 1.0);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
                    camera->giveImpulse(sf::Vector3f(0, 0.001, 0), 1.0);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
                    camera->giveImpulse(sf::Vector3f(0, 0, 0.001), 1.0);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
                    camera->giveImpulse(sf::Vector3f(0, 0, -0.001), 1.0);
                }
			}
			if (event.type == sf::Event::MouseButtonPressed) {

			}
		}

		elapsed_time = elap_time(); // Handle time
		delta_time = elapsed_time - current_time;
		current_time = elapsed_time;
		if (delta_time > 0.05f)
			delta_time = 0.05f;
		accumulator_time += delta_time;

		while (accumulator_time >= physic_step) { // While the frame has sim time, update 
		
			accumulator_time -= physic_step;
			physic_time += physic_step;

			// Do physics at 60fps
		}


        camera->update(delta_time);
		raycaster.March();

		window.clear(sf::Color::White);

		raycaster.Draw(&window);

		fps.draw(&window);
		fps.frame(delta_time);

		window.display();



	}
	return 0;

}