#pragma once
#include <unordered_map>
#include <vector>

#include "cinder/Color.h"
#include "cinder/Cinder.h"
#include "cinder/gl/gl.h"

#include "brick.h"
#include "gameState.h"
#include "intersection.h"

class Brick;
class GameState;

class Arena
{
public:
    Arena(GameState& game);

    void removeBrick(const AABB* boundingBox);

    bool isLevelClear();

    void loadLevel(int level);
    void draw();

    bool isBonusExitOpen = false;

private:
    friend class Ball;
    friend class GameState;
    friend class Laser;
    friend class Paddle;

    struct Wall
    {
        glm::vec2 topLeft;
        glm::vec2 botRight;
        AABB boundingBox;
    };

    const int MaxBricksPerLevel = 160;
    int bricksToClear = 0;

    GameState& game;

    ci::Rectf playArea{ {2, 9}, {98, 75} };

    Wall exit;
    std::vector<Wall> walls;
    std::vector<Brick> bricks;
    std::unordered_map<AABB, Brick*> AABBToBrick;

    ci::Rectf bonusExit{ {98, 67}, {100, 73} };

    ci::Color color = ci::Color(0.2f, 0.2f, 1.0f);
};
