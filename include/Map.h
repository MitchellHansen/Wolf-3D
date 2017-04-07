#pragma once
#include <vector>
#include <SFML/System.hpp>
#include <fstream>
#include <iostream>


class Map {


public:

    Map();
    ~Map();

    void Init(sf::Vector3i dimensions);

    bool loadGrid(std::string filename);
    void saveGrid(std::string filename);

    void setGrid(sf::Vector3i position, char value);
    char getGrid(sf::Vector3i position);

    sf::Vector3i getDimensions();

private:

    sf::Vector3i grid_dimensions;
    //std::vector<std::vector<std::vector<char>>> grid;
	char *grid;


};