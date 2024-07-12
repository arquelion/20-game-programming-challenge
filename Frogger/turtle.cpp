#include "precomp.h"

#include "turtle.h"

#include "gameState.h"

using namespace ci;

Turtle::Turtle(TurtlePod count, float speed, Clock::duration sinkRate)
    : count(count)
    , sinkRate(sinkRate)
    , sinkState(SinkState::EMERGED)
    , stateStartTime(game.clock.now())
    , stateDuration(sinkRate)
{
    // TODO: load textures
    glm::vec2 radius{ 4.f, 3.5f };
    float initialY = 0.f;
    switch (count)
    {
    case TurtlePod::DOUBLE:
        initialY = game.arena->water.getTopLeft().y + 15.f;
        radius.x *= 2;
        break;
    case TurtlePod::TRIPLE:
        initialY = game.arena->water.getTopLeft().y + 45.f;
        radius.x *= 3;
        break;
    }

    initialCenter = { game.arena->playArea.x2 + radius.x, initialY };
    sprite = BoundedRect::create(initialCenter, radius);
    sprite.getBoundingBox().radius.x -= Frog::collisionRadius.x * 0.5f;
    velocity = glm::vec2(speed, 0);

    color = Color(0.827f / 3 * sinkState, 0.122f, 0.176f);
}

void Turtle::update()
{
    if (sinkRate != sinkRate.zero())
    {
        if (game.clock.now() > stateStartTime + stateDuration)
        {
            sinkState += (isSubmerging) ? -1 : 1;
            stateStartTime = game.clock.now();
            stateDuration = sinkRate;
            isCollideable = true;
            color = Color(0.827f / 3 * sinkState, 0.122f, 0.176f);
            if (sinkState == SinkState::EMERGED)
            {
                isSubmerging = true;
            }
            else if (sinkState == SinkState::SUBMERGED)
            {
                isSubmerging = false;
                stateDuration = std::chrono::milliseconds(100);
                isCollideable = false;
                color = Color(0.122f, 0.827f, 0.176f);
            }
        }
    }

    Object2D::update();
}
