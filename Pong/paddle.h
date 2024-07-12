#pragma once
#include "cinder/Color.h"
#include "cinder/Cinder.h"

#include "arena.h"

class Paddle
{
public:
    Paddle() {}
    Paddle(std::shared_ptr<Arena> arena, float xLeftNorm, float yTopNorm)
        : arena(arena)
        , initialTopLeft(xLeftNorm, yTopNorm)
        , topLeft(xLeftNorm, yTopNorm)
        , botRight(topLeft + size) {}

    void keyMoveUp();
    void keyMoveDown();
    void mouseMove(float deltaY, float deltaT);
    void moveUp(float deltaY);
    void moveDown(float deltaY);

    void reset();
    void updateVelocity();
    void draw();

private:
    friend class Ball;
    friend class AIPlayer;

    std::shared_ptr<Arena> arena;

    glm::vec2 size = { 0.01f, 0.10f };
    glm::vec2 topLeft = { 0, 0 };
    glm::vec2 botRight = { 0, 0 };

    glm::vec2 initialTopLeft;

    static const float vMax;
    static const float vMin;
    float velocity = 0.f;
    float deltaV = vMin;

    int idleCount = 0;

    ci::Color color = ci::Color(0.1f, 1.0f, 0.1f);
};
