#include "Animator.h"
#include <cassert>

int main() {
    Animator a(4, 0.1f);
    int f = a.getFrame();
    a.update(0.11f);
    assert(a.getFrame() == (f+1)%4);
    for(int i=0;i<10;i++) a.update(0.11f);
    assert(a.getFrame() >=0 && a.getFrame()<4);
    return 0;
}
