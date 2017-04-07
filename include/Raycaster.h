
#include <unordered_map>
#include <SFML/Graphics/Image.hpp>
#include <unordered_map>
#include <memory>
#include "Map.h"
#include "Camera.h"
#include "util.hpp"
#include <thread>

class Raycaster {

public:

    Raycaster(std::shared_ptr<Map> map, std::shared_ptr<Camera> camera);
    ~Raycaster();

    void LoadTextures();

    void March();
    void Cast();
    void Draw(sf::RenderWindow *window);

    void CreateViewport(sf::Vector2i resolution, sf::Vector2f fov);

    void Blit(sf::Color color, sf::Vector2i position, sf::Vector3i mask);

private:

    void MarchThread(sf::Vector2i start, sf::Vector2i end);
    void MarchSingle(sf::Vector2i start, sf::Vector2i end);

    sf::Vector2i viewport_resolution;

    std::vector<sf::Vector4f> viewport_matrix;

    std::vector<sf::Uint8> viewport_image;
    sf::Texture viewport_texture;
    sf::Sprite viewport_sprite;

    std::vector<sf::Image> tile_map;
    std::vector<sf::Image> sprite_map;

    std::shared_ptr<Map> map;
    std::shared_ptr<Camera> camera;

    std::vector<std::thread> thread_pool;

};


