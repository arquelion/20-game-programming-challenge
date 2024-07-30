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
    Surface bitmap(loadImage("turtles.png"));

    Area area;
    Surface turtle;
    glm::vec2 radius{ 4.f, 4.f };
    int widthPx = 0;
    float initialY = 0.f;
    switch (count)
    {
    case TurtlePod::DOUBLE:
        initialY = game.arena->water.getTopLeft().y + 15.f;
        radius.x *= 2;
        widthPx = 32;
        break;
    case TurtlePod::TRIPLE:
        initialY = game.arena->water.getTopLeft().y + 45.f;
        radius.x *= 3;
        widthPx = 48;
        break;
    }

    initialCenter = { game.arena->playArea.x2 + radius.x, initialY };
    sprite = BoundedRect::create(initialCenter, radius);
    sprite.getBoundingBox().radius.x -= Frog::collisionRadius.x * 0.5f;
    velocity = glm::vec2(speed, 0);

    anim.container = Rectf(sprite.getTopLeft(), sprite.getBotRight());
    for (int i = 0; i < 3; ++i)
    {
        area = Area(Rectf({ 0, 16 * i }, { widthPx, 16 * (i + 1) }));
        turtle = Surface(widthPx, 16, true);
        turtle.copyFrom(bitmap, area, glm::vec2(0, -16 * i));
        anim.frames.push_back(gl::Texture2d::create(turtle));
    }
    for (int i = 3; i > 0; --i)
    {
        area = Area(Rectf({ 0, 16 * i }, { widthPx, 16 * (i + 1) }));
        turtle = Surface(widthPx, 16, true);
        turtle.copyFrom(bitmap, area, glm::vec2(0, -16 * i));
        anim.frames.push_back(gl::Texture2d::create(turtle));
    }
    anim.setDuration({sinkRate, sinkRate, sinkRate, std::chrono::milliseconds(100), sinkRate, sinkRate});
    anim.addFrameBehavior(3, [this]() { this->isCollideable = false; });
    anim.addFrameBehavior(4, [this]() { this->isCollideable = true; });

    if (sinkRate != sinkRate.zero())
    {
        anim.loop();
    }

    color = Color(0.827f / 3 * sinkState, 0.122f, 0.176f);
}

/*void Turtle::update(float deltaSec)
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

    Object2D::update(deltaSec);
}*/
