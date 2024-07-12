#include <algorithm>

#include "cinder/gl/gl.h"

#include "paddle.h"
#include "utils.h"
#include "view.h"

using namespace ci;

const float Paddle::vMax = 0.05f;
const float Paddle::vMin = 0.005f;

void Paddle::keyMoveUp()
{
    velocity = std::clamp(velocity - deltaV, -vMax, -vMin);
    moveUp(velocity);
}

void Paddle::keyMoveDown()
{
    velocity = std::clamp(velocity + deltaV, vMin, vMax);
    moveDown(velocity);
}

void Paddle::mouseMove(float deltaY, float deltaT)
{
    velocity = deltaY / deltaT;
    if (signum(velocity) == -1)
    {
        moveUp(deltaY);
    }
    else if (signum(velocity) == 1)
    {
        moveDown(deltaY);
    }
}

void Paddle::moveUp(float deltaY)
{
    topLeft.y += deltaY;
    topLeft.y = std::max(topLeft.y, arena->upperWallLeft.y + arena->arenaStrokeWidth / 2 + 0.01f);
    botRight = topLeft + size;
    idleCount = 0;
}

void Paddle::moveDown(float deltaY)
{
    botRight.y += deltaY;
    botRight.y = std::min(botRight.y, arena->lowerWallRight.y - arena->arenaStrokeWidth / 2 - 0.01f);
    topLeft = botRight - size;
    idleCount = 0;
}

void Paddle::reset()
{
    topLeft = initialTopLeft;
    botRight = topLeft + size;
}

void Paddle::updateVelocity()
{
    idleCount++;

    if (idleCount > 5)
    {
        velocity = signum(velocity) * std::clamp(fabs(velocity) - deltaV, vMin, vMax);
    }
}

void Paddle::draw()
{
    gl::color(color);
    gl::drawSolidRect(view.getRect(topLeft, botRight));
}
