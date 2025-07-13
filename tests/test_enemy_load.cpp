#include "Enemy.h"
#include <cassert>

int main(){
    Enemy e;
    assert(!e.load("no_such_file.png"));
    return 0;
}
