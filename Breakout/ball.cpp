#include "ball.h"
#include "view.h"
#include "utils.h"

using namespace ci;

Ball::Ball(GameState& game)
    : game(game)
    , startSpeed(40)
    , maxSpeed(80)
    , speedBump(8)
{
    reset();
}

Ball::Intersect Ball::checkForIntersect(float deltaSec) const
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

    // Bottom of the screen
    auto sweep = game.arena->exit.boundingBox.sweepAABB(this->boundingBox, dir);
    if (sweep.t != 1.f && sweep.hit->t < closest.hit->t)
    {
        closest = sweep;
        nextCollisionObject = CollisionObject::EXIT;
    }

    if (game.isActive)
    {
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

        // Paddle
        auto sweep = game.paddle->boundingBox.sweepAABB(this->boundingBox, dir);
        if (sweep.t != 1.f && sweep.hit->t < closest.hit->t)
        {
            closest = sweep;
            nextCollisionObject = CollisionObject::PADDLE;
        }
    }

    return { closest, nextCollisionObject };
}

void Ball::increaseVelocity()
{
    auto dirNorm = glm::normalize(velocity);
    auto newSpeed = std::min(maxSpeed, speedBump + glm::length(velocity));
    velocity = newSpeed * dirNorm;
}

void Ball::reset()
{
    center = game.paddle->center;
    center.y -= game.paddle->radius.y + radius + epsilon;

    auto angle = lerp(-M_PI / 4, M_PI / 4, getRandomNum<float>());
    velocity = startSpeed * glm::vec2(sin(angle), -cos(angle));

    boundingBox.center = center;
    boundingBox.radius = { radius, radius };

    autoReleaseTime = game.clock.current() + std::chrono::days(1);
    isAttached = true;
}

void Ball::move(float deltaSec)
{
    if (isAttached && game.isActive)
    {
        if (autoReleaseTime > game.clock.current())
        {
            center = game.paddle->center;
            center.x -= catchOffset;
            center.y -= game.paddle->radius.y + radius + epsilon;
            boundingBox.center = center;
            return;
        }
        else
        {
            release();
        }
    }

    do
    {
        if (deltaSec <= 0) return;

        auto intersect = checkForIntersect(deltaSec);
        auto& hit = intersect.sweep.hit;
        float radians = 0;

        center += velocity * deltaSec;

        switch (intersect.collider)
        {
        case CollisionObject::NONE:
            boundingBox.center = center;
            return;
        case CollisionObject::BRICK:
            if (!game.isActive)
            {
                boundingBox.center = center;
                return;
            }
            game.arena->removeBrick(hit->box);
            break;
        case CollisionObject::PADDLE:
            if (!game.isActive)
            {
                boundingBox.center = center;
                return;
            }

            // Modify direction based on where collision was on the paddle unless the collision is with the side
            if (hit->normal.y == -1.f)
            {
                velocity = -glm::length(velocity) * hit->normal;
                auto offset = hit->pos.x - game.paddle->center.x;
                if (fabs(offset) >= game.paddle->radius.x * 0.8)
                {
                    radians = 2 * glm::pi<float>() / 6 * signum(offset);
                }
                else if (fabs(offset) >= game.paddle->radius.x * 0.5)
                {
                    radians = glm::pi<float>() / 4 * signum(offset);
                }
                else
                {
                    radians = glm::pi<float>() / 6 * signum(offset);
                }
            }
            break;
        case CollisionObject::EXIT:
            if (game.isActive)
            {
                isOffField = true;
                return;
            }
            break;
        }

        velocity = glm::reflect(velocity, hit->normal);
        velocity = glm::rotate(velocity, radians);
        increaseVelocity();
        center += hit->delta + hit->normal * epsilon;
        boundingBox.center = center;
        deltaSec -= hit->t * deltaSec;
        game.bounceSound->start();

        if (game.paddle->isCatchEnabled && intersect.collider == CollisionObject::PADDLE)
        {
            autoReleaseTime = game.clock.current() + std::chrono::milliseconds(2000);
            catchOffset = game.paddle->center.x - center.x;
            isAttached = true;
            return;
        }
    } while (true);
}

void Ball::release()
{
    if (isAttached)
    {
        isAttached = false;
    }
}

void Ball::draw() const
{
    gl::color(color);
    gl::drawSolidRect(view.getRect(center - radius, center + radius));
}
