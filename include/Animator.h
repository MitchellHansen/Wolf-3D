#pragma once
class Animator {
public:
    Animator(int frameCount=1, float frameTime=0.2f);
    void update(float dt);
    int getFrame() const { return currentFrame; }
private:
    int frames;
    float frameTime;
    float elapsed{0.f};
    int currentFrame{0};
};
