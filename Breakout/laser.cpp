#include "laser.h"

using namespace ci;

int Laser::nextId = 1;

Laser::Laser(GameState& game, glm::vec2 start)
    : game(game)
{
    id = nextId++;
    center.x = start.x;
    center.y = start.y - radius.y;
}

Laser::Laser(Laser&& src)
    : game(src.game)
{
    *this = std::move(src);
}

bool Laser::hasExited() const
{
    return (center.y - radius.y) < game.arena->playArea.y1 || didHitBrick;
}

void Laser::move(float deltaSec)
{
    auto intersect = checkForIntersect(deltaSec);
    auto& hit = intersect.sweep.hit;
    float radians = 0;
    switch (intersect.collider)
    {
    case CollisionObject::NONE:
        center += velocity * deltaSec;
        boundingBox.center = center;
        break;
    case CollisionObject::BRICK:
        game.arena->removeBrick(hit->box);
        didHitBrick = true;
        break;
    }
}

void Laser::draw() const
{
    gl::color(color);
    gl::drawSolidRoundedRect(view.getRect(center - radius, center + radius), view.calcScaledValue(0.5f));
}

Laser::Intersect Laser::checkForIntersect(float deltaSec) const
{
    auto dir = velocity * deltaSec;

    Sweep closest;
    closest.hit = std::make_optional(this->boundingBox);
    closest.hit->t = FLOAT_INF;
    CollisionObject nextCollisionObject = CollisionObject::NONE;

    // Walls
    for (auto& wall : game.arena->walls)
    {
        auto sweep = wall.boundingBox.sweepAABB(this->boundingBox, dir);
        if (sweep.t != 1.f && sweep.hit->t < closest.hit->t)
        {
            closest = sweep;
            nextCollisionObject = CollisionObject::WALL;
        }
    }

    // Bricks
    for (auto& brick : game.arena->bricks)
    {
        auto sweep = brick.boundingBox.sweepAABB(this->boundingBox, dir);
        if (sweep.t != 1.f && sweep.hit->t < closest.hit->t)
        {
            closest = sweep;
            nextCollisionObject = CollisionObject::BRICK;
        }
    }

    return { closest, nextCollisionObject };
}
