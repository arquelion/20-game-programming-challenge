#pragma once
#include <chrono>

#include "cinder/Color.h"
#include "cinder/Cinder.h"

#include "arena.h"
#include "gameState.h"
#include "intersection.h"
#include "powerup.h"

class Paddle
{
public:
    Paddle(GameState& game) : game(game) {}
    Paddle(GameState& game, glm::vec2 center);

    void fireLaser() const;

    void move(glm::vec2 newPos);
    void reset();
    void draw() const;

    bool isCatchEnabled = false;
    bool isLaserEnabled = false;
    bool wentThroughBonusExit = false;

private:
    friend class Ball;
    friend class GameState;
    friend class Powerup;

    GameState& game;

    glm::vec2 center;
    glm::vec2 radius{ 6, 1 };

    glm::vec2 initialCenter;
    glm::vec2 initialRadius;

    AABB boundingBox;
    ci::Color inner;
    ci::Color middle;
    ci::Color outer;
    ci::Color initialInner;
    ci::Color initialMiddle;
    ci::Color initialOuter;

    void nudge(Ball& ball) const;
};
