
#include <unordered_map>
#include <SFML/Graphics/Image.hpp>
#include <unordered_map>
#include <memory>
#include "Map.h"
#include "Camera.h"
#include "util.hpp"
#include <thread>
#include <cmath>
#include <queue>
#include <mutex>


struct F_Vec4f {
	
	F_Vec4f(){}
	F_Vec4f(sf::Vector4f in) : x(in.x), y(in.y), z(in.z), w(in.w) {}
	void noCopy(float x, float y, float z, float w) {
		this->x = x; this->y = y; this->z = z; this->w = w;
	}

	float x;
	float y;
	float z;
	float w;

};
class Raycaster {

public:

    Raycaster(Map *map, std::shared_ptr<Camera> camera);
    ~Raycaster();

    void LoadTextures();

    void March();
    void Cast();
    void Draw(sf::RenderWindow *window);

    void CreateViewport(sf::Vector2i resolution, sf::Vector2f fov);

    void Blit(sf::Color color, sf::Vector2i position, sf::Vector3i mask);


private:

    void MarchThread();
    void MarchSingle(sf::Vector2i start, sf::Vector2i end);

    sf::Vector2i viewport_resolution;

    sf::Vector4f *viewport_matrix;

    sf::Uint8 *viewport_image;
    sf::Texture viewport_texture;
    sf::Sprite viewport_sprite;

    float fov_horizontal = 60.f;

    std::vector<sf::Image> tile_map;
    std::vector<sf::Image> sprite_map;

    Map *map;
    std::shared_ptr<Camera> camera;

    std::vector<std::thread> thread_pool;

	std::mutex queue_mutex;
	std::queue<std::pair<sf::Vector2i, sf::Vector2i>> block_queue;

	bool end_state = false;
};


