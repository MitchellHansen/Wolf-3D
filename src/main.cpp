
#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>
#include <Camera.h>
#include <Map.h>
#include <Raycaster.h>
#include "Enemy.h"
#include "util.hpp"
#include <vector>

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

        sf::RenderWindow window(sf::VideoMode(800, 800), "Wolf-3D");
        window.setMouseCursorVisible(false);
        window.setMouseCursorGrabbed(true);
        sf::Vector2i windowCenter(window.getSize().x / 2, window.getSize().y / 2);
        sf::Mouse::setPosition(windowCenter, window);


    std::shared_ptr<Camera> camera(new Camera);
    camera->setPosition(sf::Vector3f(3.1f, 3.1f, 3.1f));
    camera->setDirection(sf::Vector2f(1.0f, 0.0f));

    Map *map = new Map;

    if (!map->loadGrid("grid.bin")){
        map->Init(sf::Vector3i(30, 50, 5));
    }

    Raycaster raycaster(map, camera);

    raycaster.CreateViewport(sf::Vector2i(800, 800), sf::Vector2f(80.0f, 80.0f));
    raycaster.LoadTextures();

    sf::Vector3i dims = map->getDimensions();
    std::uniform_real_distribution<float> xdist(0.f, static_cast<float>(dims.x));
    std::uniform_real_distribution<float> ydist(0.f, static_cast<float>(dims.y));
    float scale = 800.f / static_cast<float>(std::max(dims.x, dims.y));

    auto toScreen = [&](sf::Vector2f world){
        return sf::Vector2f(world.x * scale, world.y * scale);
    };

    std::vector<Enemy> enemies;

    // Spawn one enemy directly in front of the player
    {
        Enemy e;
        e.load("assets/elite-guard.png");
        sf::Vector3f camPos = camera->getPosition();
        sf::Vector3f dir = camera->getDirectionCartesian();
        sf::Vector2f wpos(camPos.x + dir.x * 2.f, camPos.y + dir.y * 2.f);
        e.setPosition(toScreen(wpos));
        enemies.push_back(e);
    }

    for (int i = 0; i < 4; ++i) {
        Enemy e;
        e.load("assets/elite-guard.png");
        sf::Vector2f wpos(xdist(rng), ydist(rng));
        e.setPosition(toScreen(wpos));
        e.setVelocity(sf::Vector2f(20.f, 0.f));
        enemies.push_back(e);
    }


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
                                return 1;
                        }
                        if (event.type == sf::Event::MouseMoved) {
                                sf::Vector2i pos = sf::Mouse::getPosition(window);
                                int dx = pos.x - windowCenter.x;
                                int dy = pos.y - windowCenter.y;
                                if (dx != 0 || dy != 0) {
                                        camera->moveDirection(sf::Vector2f(dy * 0.0004f, dx * 0.0004f));
                                        std::cout << "Mouse delta: " << dx << "," << dy << std::endl;
                                        sf::Mouse::setPosition(windowCenter, window);
                                }
                        }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                        camera->giveImpulse(sf::Vector3f(0.0025f, 0, 0), 1.0);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                        camera->giveImpulse(sf::Vector3f(0, -0.0025f, 0), 1.0);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                        camera->giveImpulse(sf::Vector3f(-0.0025f, 0, 0), 1.0);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                        camera->giveImpulse(sf::Vector3f(0, 0.0025f, 0), 1.0);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                        camera->giveImpulse(sf::Vector3f(0, 0, 0.0025f), 1.0);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                        camera->giveImpulse(sf::Vector3f(0, 0, -0.0025f), 1.0);
                }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			camera->moveDirection(sf::Vector2f(0, -0.1));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			camera->moveDirection(sf::Vector2f(0, 0.1));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			camera->moveDirection(sf::Vector2f(0.1, 0));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			camera->moveDirection(sf::Vector2f(-0.1, 0));
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


        
                raycaster.Cast();


                camera->update(delta_time, map);

		window.clear(sf::Color::White);

                raycaster.Draw(&window);

                for (auto &enemy : enemies) {
                    enemy.update(static_cast<float>(delta_time));
                    enemy.draw(window, *camera, 60.f);
                }

                fps.draw(&window);
                fps.frame(delta_time);

		window.display();



	}
	return 0;

}