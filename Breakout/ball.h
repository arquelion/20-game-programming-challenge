#pragma once
#include <memory>

#include "cinder/gl/gl.h"

#include "brick.h"
#include "clock.h"
#include "gameState.h"
#include "intersection.h"

enum class CollisionObject;
class GameState;

class Ball
{
public:
    Ball(GameState& game);

    bool hasExited() { return isOffField; }

    void reset();
    void move(float deltaSec);
    void release();

    void draw() const;

private:
    friend class GameState;
    friend class Paddle;

    struct Intersect
    {
        Sweep sweep;
        CollisionObject collider;
    };

    GameState& game;

    glm::vec2 center;
    glm::vec2 velocity;
    float startSpeed;
    float maxSpeed;
    float speedBump;

    bool isAttached = true;
    float catchOffset = 0.f;
    Clock::time_point autoReleaseTime;

    const float radius = 1.f;

    AABB boundingBox;
    ci::Color color = ci::Color(0.2f, 0.2f, 1.0f);

    bool isOffField = false;

    Intersect checkForIntersect(float deltaSec) const;
    void increaseVelocity();
};
