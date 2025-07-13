#include "Animator.h"

Animator::Animator(int frameCount, float frameTime)
    : frames(frameCount), frameTime(frameTime) {}

void Animator::update(float dt) {
    elapsed += dt;
    if (elapsed >= frameTime && frames > 0) {
        elapsed -= frameTime;
        currentFrame = (currentFrame + 1) % frames;
    }
}
