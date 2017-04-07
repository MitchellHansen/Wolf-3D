#include <Map.h>


Map::Map(){

}
Map::~Map(){

}

void Map::Init(sf::Vector3i dimensions){

    grid_dimensions = dimensions;

    grid.resize(dimensions.x);
    for (auto &x: grid) {
        x.resize(dimensions.y);

        for (auto &q: x)
            q.resize(dimensions.z);
    }

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

    grid.resize(x_dim);
    for (auto &x: grid) {
        x.resize(y_dim);

        for (auto &q: x)
            q.resize(z_dim);
    }

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

    int x_dim = grid.size();
    int y_dim = grid.at(0).size();
    int z_dim = grid.at(0).at(0).size();

    file.write((char*)&x_dim, sizeof(int));
    file.write((char*)&y_dim, sizeof(int));
    file.write((char*)&z_dim, sizeof(int));

    for (int x = 0; x < x_dim; ++x){
        for (int y = 0; y < y_dim; ++y){
            for (int z = 0; z < z_dim; ++z) {
                char val = getGrid(sf::Vector3i(x, y, z));
                file.write(&val, sizeof(char));
            }
        }
    }
}

void Map::setGrid(sf::Vector3i position, char value) {

    grid.at(position.x).at(position.y).at(position.z) = value;

}
char Map::getGrid(sf::Vector3i position){

    return grid[position.x][position.y][position.z];

}

sf::Vector3i Map::getDimensions() {
    return grid_dimensions;
}












