#include "Map.h"
#include <cassert>

int main(){
    Map map;
    map.Init(sf::Vector3i(3,2,4)); // dimensions x=3,y=2,z=4
    map.setGrid(sf::Vector3i(2,1,3), 42);
    char val = map.getGrid(sf::Vector3i(2,1,3));
    assert(val == 42);
    return 0;
}
