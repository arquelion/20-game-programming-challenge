#pragma once
#include "cinder/Color.h"
#include "cinder/Cinder.h"
#include "cinder/gl/gl.h"

#include "gameState.h"
#include "intersection.h"

class GameState;

enum class BrickColor
{
    WHITE,
    ORANGE,
    CYAN,
    GREEN,
    RED,
    BLUE,
    MAGENTA,
    YELLOW,
    SILVER,
    GOLD,
};

class Brick
{
public:
    Brick(GameState& game) : game(game) {}
    Brick(GameState& game, BrickColor color, glm::vec2 center, int level);

    BrickColor getColor() { return color; }
    void hit();

    void draw();

private:
    friend class Arena;
    friend class Ball;
    friend class Laser;

    GameState& game;

    bool isActive = true;
    int hitsRemaining = 1;

    BrickColor color;

    glm::vec2 center;
    glm::vec2 radius{ 3, 1 };

    AABB boundingBox;
};
