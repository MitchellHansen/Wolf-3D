#include <Map.h>


Map::Map(){
    grid = nullptr;
}
Map::~Map(){
    delete[] grid;
    grid = nullptr;
}

void Map::Init(sf::Vector3i dimensions){

    grid_dimensions = dimensions;

	grid = new char[grid_dimensions.x * grid_dimensions.y * grid_dimensions.z];

    for (int x = 0; x < dimensions.x; ++x){
        for (int y = 0; y < dimensions.y; ++y){
            for (int z = 0; z < dimensions.z; ++z) {

                char val = 0;
                if (rand() % 10 > 8)
                    val = 1;
                setGrid(sf::Vector3i(x, y, z), val);
            }
        }
    }

}

bool Map::loadGrid(std::string filename){

    std::ifstream file(filename);
    if (!file.is_open()){
        std::cout << "Cant open file : " << filename;
        return false;
    }

    int x_dim = 0;
    int y_dim = 0;
    int z_dim = 0;

    file.read((char*)&x_dim, sizeof(int));
    file.read((char*)&y_dim, sizeof(int));
    file.read((char*)&z_dim, sizeof(int));

    grid = new char[x_dim * y_dim * z_dim];

    for (int x = 0; x < x_dim; ++x){
        for (int y = 0; y < y_dim; ++y){
            for (int z = 0; z < z_dim; ++z) {
                char val = 0;
                file.read(&val, sizeof(char));
                setGrid(sf::Vector3i(x, y, z), val);
            }
        }
    }

    grid_dimensions = sf::Vector3i(x_dim, y_dim, z_dim);

    return true;
}

void Map::saveGrid(std::string filename) {

    std::ofstream file(filename);
    if (!file.is_open()){
        std::cout << "Cant open file : " << filename;
        return;
    }

    file.write((char*)&grid_dimensions.x, sizeof(int));
    file.write((char*)&grid_dimensions.y, sizeof(int));
    file.write((char*)&grid_dimensions.z, sizeof(int));

    for (int x = 0; x < grid_dimensions.x; ++x){
        for (int y = 0; y < grid_dimensions.y; ++y){
            for (int z = 0; z < grid_dimensions.z; ++z) {
                char val = getGrid(sf::Vector3i(x, y, z));
                file.write(&val, sizeof(char));
            }
        }
    }
}

void Map::setGrid(sf::Vector3i position, char value) {

    grid[position.x + grid_dimensions.x * (position.y + grid_dimensions.y * position.z)] = value;
   // grid.at(position.x).at(position.y).at(position.z) = value;

}
char Map::getGrid(sf::Vector3i position){

    return grid[position.x + grid_dimensions.x * (position.y + grid_dimensions.y * position.z)];
    //;return grid[position.x][position.y][position.z];

}

sf::Vector3i Map::getDimensions() {
    return grid_dimensions;
}

bool Map::isWall(sf::Vector3f position) {
    int x = static_cast<int>(position.x);
    int y = static_cast<int>(position.y);

    if (x < 0 || y < 0 ||
        x >= grid_dimensions.x || y >= grid_dimensions.y)
        return true;

    // Collision is checked in the first layer since the raycaster
    // only renders that plane.
    return getGrid(sf::Vector3i(x, y, 0)) != 0;
}












