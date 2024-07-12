#pragma once
#include <memory>

#include "cinder/gl/gl.h"

#include "gameState.h"

enum class Player;
class GameState;

class Ball
{
public:
    Ball(GameState& game);

    void reset();
    void move(float deltaSec);
    void draw() const;

private:
    friend class AIPlayer;

    enum class CollisionObject
    {
        P1PADDLE,
        P2PADDLE,
        WALL,
    };

    GameState& game;
    Player lastScored;

    glm::vec2 pos;
    glm::vec2 velocity;

    const float radius = 0.005f;
    const glm::vec2 vMax = { 0.05f, 0.05f };
    const glm::vec2 vMin = { 0.001f, 0.001f };

    void updateVelocity(CollisionObject object);
};
