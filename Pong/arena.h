#pragma once
#include "cinder/Color.h"
#include "cinder/Cinder.h"
#include "cinder/gl/gl.h"

class Arena
{
public:
    Arena() {}

    void draw();

private:
    friend class Ball;
    friend class Paddle;

    float arenaStrokeWidth = 0.005f;

    // Upper wall norms
    glm::vec2 upperWallLeft = { 0.f, 0.2f };
    glm::vec2 upperWallRight = { 1.f, 0.2f };

    // Lower wall norms
    glm::vec2 lowerWallLeft = { 0, 1.f - arenaStrokeWidth / 2 };
    glm::vec2 lowerWallRight = { 1.f, 1.f - arenaStrokeWidth / 2 };

    // Net norms
    glm::vec2 netTop = { 0.5f, 0.2f };
    glm::vec2 netBot = { 0.5f, 1.f };

    ci::Color color = ci::Color(0.1f, 1.0f, 0.1f);
};
