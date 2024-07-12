#pragma once
#include <chrono>
#include <memory>

#include "cinder/gl/gl.h"

#include "brick.h"
#include "gameState.h"
#include "intersection.h"
#include "view.h"

enum class CollisionObject;

class Laser
{
public:
    Laser(GameState& game, glm::vec2 start);
    Laser(Laser&& src);

    Laser& operator=(Laser&& rhs) noexcept
    {
        id = std::move(rhs.id);
        center = std::move(rhs.center);
        didHitBrick = std::move(rhs.didHitBrick);
        boundingBox = std::move(rhs.boundingBox);
        color = std::move(rhs.color);
        return *this;
    }

    bool operator==(const Laser& rhs) const
    {
        return this->id == rhs.id;
    }

    bool hasExited() const;

    void move(float deltaSec);
    void draw() const;

    static int nextId;

private:
    struct Intersect
    {
        Sweep sweep;
        CollisionObject collider;
    };

    int id = 0;

    GameState& game;

    glm::vec2 center;
    const glm::vec2 radius{ 0.4f, 1.f };
    const glm::vec2 velocity{ 0.f, -50.f };

    bool didHitBrick = false;

    AABB boundingBox;
    ci::Color color = ci::Color(0.2f, 0.2f, 1.0f);

    Intersect checkForIntersect(float deltaSec) const;
};
