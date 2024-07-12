#pragma once
#include <chrono>

#include "cinder/gl/gl.h"

#include "paddle.h"

class MousePlayer
{
public:
    MousePlayer(Paddle& paddle) : paddle(paddle) {}

    void move(glm::vec2 pos, std::chrono::steady_clock::time_point time);

private:
    Paddle& paddle;

    glm::vec2 lastPos;
    std::chrono::steady_clock::time_point lastTimeMoved;
};
